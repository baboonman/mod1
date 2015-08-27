#include "OpenCL.hpp"

OpenCL::OpenCL(cl_int nbParticle, cl_float sizeGridCoef, cl_int maxParticlePerCell, cl_int gridX, cl_int gridY, cl_int gridZ) 
: _nbParticle(nbParticle), _sizeGridCoef(sizeGridCoef), _maxParticlePerCell(maxParticlePerCell), _maxWorkItemSize(NULL)
{
    std::cout << "Nb particle: " << this->_nbParticle << std::endl;
    this->_gridSize[OpenCL::X] = gridX;
    this->_gridSize[OpenCL::Y] = gridY;
    this->_gridSize[OpenCL::Z] = gridZ;
    this->_maxGid = this->_nbParticle - 1;
}

void    OpenCL::_initTask() {
    cl_kernel       kernel;

    this->_taskParticleInGrid = new TaskParticleInGrid(this->_context, this->_device, this->_nbParticle);
    this->_taskParticleInGrid->createKernel();
    this->_taskApplyForces = new TaskApplyForces(this->_context, this->_device, this->_nbParticle);
    this->_taskApplyForces->createKernel();
}

void    OpenCL::initOpenCL() {
    this->_createContext();
    this->_getDeviceInfo();
    this->_bindBuffer();
    this->_initTask();
//    this->_bindVBO();
    this->_createCommandQueue();
    this->_setKernelArg();
}

void    OpenCL::_createContext() {
    cl_int      err;

    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

    cl_context_properties properties[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)kCGLShareGroup,
        0
    };

    this->_context = clCreateContext(properties, 0, 0, NULL, NULL, &err);
    checkCLSuccess(err, "clCreateContext");
    checkCLSuccess(clGetContextInfo(this->_context,
                CL_CONTEXT_NUM_DEVICES,
                sizeof(cl_int),
                &(this->_nbDevice),
                NULL),
        "clGetContextInfo");

    if (this->_nbDevice < 1) {
        throw new OpenCLException();
    }

    checkCLSuccess(clGetContextInfo(this->_context,
                CL_CONTEXT_DEVICES,
                sizeof(cl_device_id),
                &(this->_device),
                NULL),
        "clGetContextInfo");
}

void    OpenCL::_getDeviceInfo() {
    size_t      size;
    char        *deviceName;

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_NAME,
                0,
                NULL,
                &size),
        "clGetDeviceInfo size");
    deviceName = new char[size];
    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_NAME,
                size,
                deviceName,
                NULL),
            "clGetDeviceInfo info");
    std::cout << "Device: " << deviceName << std::endl;
    delete[] deviceName;

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                sizeof(cl_uint),
                &(this->_maxWorkingDimensions),
                NULL),
            "clGetDeviceInfo max working dimensions");

    this->_maxWorkItemSize = new size_t[this->_maxWorkingDimensions];

    checkCLSuccess(clGetDeviceInfo(this->_device,
                CL_DEVICE_MAX_WORK_ITEM_SIZES,
                sizeof(size_t) * this->_maxWorkingDimensions,
                this->_maxWorkItemSize,
                NULL),
            "clGetDeviceInfo max working dimensions");

    std::cout << "(";
    for (size_t i = 0; i < this->_maxWorkingDimensions; i++) {
        std::cout << this->_maxWorkItemSize[i] << ", ";
    }
    std::cout << ")" << std::endl;

}

void    OpenCL::_setStdArg(cl_kernel kernel) {
    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particle),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_mem), &this->_particleVelocity),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 2, sizeof(cl_mem), &this->_particleProjection),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 3, sizeof(cl_mem), &this->_particleIdByCells),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 4, sizeof(cl_float), &this->_sizeGridCoef),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 5, sizeof(cl_int), &this->_gridSize[OpenCL::X]),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 6, sizeof(cl_int), &this->_gridSize[OpenCL::Y]),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 7, sizeof(cl_int), &this->_gridSize[OpenCL::Z]),
            "clSetKernelArg");
    std::cout << "max particle: " << this->_maxParticlePerCell<< std::endl;
    checkCLSuccess(clSetKernelArg(kernel, 8, sizeof(cl_int), &this->_maxParticlePerCell),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 9, sizeof(cl_int), &this->_maxGid),
            "clSetKernelArg");
}

void    OpenCL::_setKernelArg() {
    cl_kernel       kernel;

    this->_setStdArg(this->_taskParticleInGrid->getKernel());
    this->_setStdArg(this->_taskApplyForces->getKernel());
}

void    OpenCL::executeKernel() {
    //int     err;

/*    err = clEnqueueAcquireGLObjects(
        this->_commandQueue,
        1,
        &this->_waterBuffer,
        0,
        NULL,
        NULL),
    checkCLSuccess(err, "clEnqueueAcquireGLObjects");
    */
    this->_taskParticleInGrid->enqueueKernel(this->_commandQueue);
    this->_taskApplyForces->enqueueKernel(this->_commandQueue);
    clFinish(this->_commandQueue);
    /*
    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_waterBuffer,
            0,
            NULL,
            NULL);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
    */
}

void    OpenCL::release() {
    clFinish(this->_commandQueue);
    /*
    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_waterBuffer,
            0,
            NULL,
            NULL);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
    clReleaseMemObject(this->_waterBuffer);
    clReleaseProgram(this->_program);
    clReleaseKernel(this->_kernel);
    clReleaseCommandQueue(this->_commandQueue);
    clReleaseContext(this->_context);
    */
}

void    OpenCL::_bindBuffer() {
    int     err;

    this->_particle = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float) * 3, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer particle position");

    this->_particleIdByCells = clCreateBuffer(this->_context,
            CL_MEM_READ_WRITE, this->_gridSize[OpenCL::X] * this->_gridSize[OpenCL::Y] * this->_gridSize[OpenCL::Z] * sizeof(cl_int) * this->_maxParticlePerCell + 1,
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer id by cell");

    this->_particleVelocity = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float) * 3, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer particle velocity");

    this->_particleProjection = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float) * 3, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer projection");
}

/*
void    OpenCL::_bindVBO() {
    cl_int     err;

    this->_waterBuffer = clCreateFromGLBuffer(this->_context, CL_MEM_READ_WRITE, this->_waterVBO, &err);
    checkCLSuccess(err, "clCreateFromGLBuffer");
}
*/

void    OpenCL::_createCommandQueue( void ) {
    int     err;

    this->_commandQueue = clCreateCommandQueue(this->_context, this->_device, 0, &err);
    checkCLSuccess(err, "clCreateCommandQueue");
}

void    OpenCL::displayInformation( void ) {
    Platform    platform;

    platform.displayInfoPlatforms();
}

OpenCL::~OpenCL( void ) {
    if (this->_maxWorkItemSize) {
        delete[] this->_maxWorkItemSize;
    }

    if (this->_taskParticleInGrid) {
        delete this->_taskParticleInGrid;
    }

    if (this->_taskApplyForces) {
        delete this->_taskApplyForces;
    }
}
