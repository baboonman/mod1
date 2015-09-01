#include "OpenCL.hpp"

OpenCL::OpenCL(cl_int nbParticle, cl_float sizeGridCoef, cl_int maxParticlePerCell, cl_int gridX, cl_int gridY, cl_int gridZ) 
: _nbParticle(nbParticle), _sizeGridCoef(sizeGridCoef), _maxParticlePerCell(maxParticlePerCell), _maxWorkItemSize(NULL)
{
    this->_gridSize[OpenCL::X] = gridX;
    this->_gridSize[OpenCL::Y] = gridY;
    this->_gridSize[OpenCL::Z] = gridZ;
    this->_maxGid = this->_nbParticle - 1;
}

void    OpenCL::_initTask() {
    this->_taskParticleInGrid = new TaskParticleInGrid(this->_context, this->_device, this->_nbParticle);
    this->_taskParticleInGrid->createKernel();

    this->_taskApplyForces = new TaskApplyForces(this->_context, this->_device, this->_nbParticle);
    this->_taskApplyForces->createKernel();

    this->_taskAddConst = new TaskAddConst(this->_context, this->_device, this->_nbParticle);
    this->_taskAddConst->createKernel();


    this->_taskInitBuffer = new TaskInitBuffer(this->_context, this->_device, this->_nbParticle);
    this->_taskInitBuffer->createKernel(this->_sizeGrid);

    this->_taskInitParticle = new TaskInitParticle(this->_context, this->_device, this->_nbParticle);
    this->_taskInitParticle->createKernel();

    this->_taskEndSim = new TaskEndSim(this->_context, this->_device, this->_nbParticle);
    this->_taskEndSim->createKernel();

    this->_taskCalcLambda = new TaskCalcLambda(this->_context, this->_device, this->_nbParticle);
    this->_taskCalcLambda->createKernel();

    this->_taskFindNeighbors= new TaskFindNeighbors(this->_context, this->_device, this->_nbParticle);
    this->_taskFindNeighbors->createKernel();
}

void    OpenCL::initOpenCL(GLuint vbo) {
    this->_createContext();
    this->_getDeviceInfo();
    this->_bindBuffer();
    this->_bindVBO(vbo);
    this->_initTask();
    this->_createCommandQueue();
    this->_setKernelArg();

    int err;
    err = clEnqueueAcquireGLObjects(
        this->_commandQueue,
        1,
        &this->_particle,
        0,
        NULL,
        NULL);
    checkCLSuccess(err, "clEnqueueAcquireGLObjects");
    this->_taskInitParticle->enqueueKernel(this->_commandQueue);
    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_particle,
            0,
            NULL,
            NULL);
    clFinish(this->_commandQueue);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
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
    checkCLSuccess(clSetKernelArg(kernel, 8, sizeof(cl_int), &this->_maxParticlePerCell),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 9, sizeof(cl_int), &this->_maxGid),
            "clSetKernelArg");
}

void    OpenCL::_setKernelConstArg(cl_kernel kernel) {
    checkCLSuccess(clSetKernelArg(kernel, 10, sizeof(cl_mem), &this->_particleLambda),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 11, sizeof(cl_mem), &this->_particleNeighbors),
            "clSetKernelArg");
}

void    OpenCL::_setKernelArg() {
    cl_kernel       kernel;

    this->_setStdArg(this->_taskParticleInGrid->getKernel());
    this->_setStdArg(this->_taskApplyForces->getKernel());
    this->_setStdArg(this->_taskAddConst->getKernel());
    this->_setStdArg(this->_taskEndSim->getKernel());
    this->_setStdArg(this->_taskCalcLambda->getKernel());
    this->_setStdArg(this->_taskFindNeighbors->getKernel());
    this->_setKernelConstArg(this->_taskAddConst->getKernel());
    this->_setKernelConstArg(this->_taskCalcLambda->getKernel());
    this->_setKernelConstArg(this->_taskFindNeighbors->getKernel());

    kernel = this->_taskInitBuffer->getKernel();

    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particleIdByCells),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_int), &this->_sizeGrid),
            "clSetKernelArg");

    kernel = this->_taskInitParticle->getKernel();

    checkCLSuccess(clSetKernelArg(kernel, 0, sizeof(cl_mem), &this->_particle),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 1, sizeof(cl_mem), &this->_particleVelocity),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(kernel, 2, sizeof(cl_int), &this->_maxGid),
            "clSetKernelArg");

}

void    OpenCL::executeKernel() {
    int     err;

    struct timeval      timeVal1;
    struct timeval      timeVal2;
    unsigned long long int  start;
    unsigned long long int  end;
    double                  time;
    unsigned long long int  time2;

    err = clEnqueueAcquireGLObjects(
        this->_commandQueue,
        1,
        &this->_particle,
        0,
        NULL,
        NULL);
    checkCLSuccess(err, "clEnqueueAcquireGLObjects");

    gettimeofday(&timeVal1, NULL);

    this->_taskInitBuffer->enqueueKernel(this->_commandQueue);
    this->_taskParticleInGrid->enqueueKernel(this->_commandQueue);
    this->_taskApplyForces->enqueueKernel(this->_commandQueue);
    this->_taskFindNeighbors->enqueueKernel(this->_commandQueue);
    for (int i = 0; i < 3; i++) {
        this->_taskCalcLambda->enqueueKernel(this->_commandQueue);
        this->_taskAddConst->enqueueKernel(this->_commandQueue);
    }
    this->_taskEndSim->enqueueKernel(this->_commandQueue);
    clFinish(this->_commandQueue);
    gettimeofday(&timeVal2, NULL);
    start = 1000000 * timeVal1.tv_sec + (timeVal1.tv_usec);
    end = 1000000 * timeVal2.tv_sec + (timeVal2.tv_usec);
    time = ((double)(end - start)) / 1000000.0;
    time2 = end - start;
    printf("Execution time: %lf ms, %llu us\n", time * 1000.0f, time2);


    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_particle,
            0,
            NULL,
            NULL);
    clFinish(this->_commandQueue);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
}

void    OpenCL::release() {
    std::cout << "OpenCL ressources release" << std::endl;
    clFinish(this->_commandQueue);
    clReleaseMemObject(this->_particle);
    clReleaseMemObject(this->_particleIdByCells);
    clReleaseMemObject(this->_particleVelocity);
    clReleaseMemObject(this->_particleProjection);
    clReleaseMemObject(this->_particleLambda);
    clReleaseMemObject(this->_particleNeighbors);

    this->_taskParticleInGrid->releaseKernel();
    this->_taskApplyForces->releaseKernel();
    this->_taskAddConst->releaseKernel();
    this->_taskInitBuffer->releaseKernel();
    this->_taskInitParticle->releaseKernel();
    this->_taskCalcLambda->releaseKernel();
    this->_taskFindNeighbors->releaseKernel();
    this->_taskEndSim->releaseKernel();

    clReleaseCommandQueue(this->_commandQueue);
    clReleaseContext(this->_context);
    std::cout << "DONE" << std::endl;
}

void    OpenCL::_bindBuffer() {
    int     err;

    this->_sizeGrid = this->_gridSize[OpenCL::X] * this->_gridSize[OpenCL::Y] * this->_gridSize[OpenCL::Z] * (this->_maxParticlePerCell + 1);
    std::cout << "size: " << this->_sizeGrid << std::endl;
    this->_particleIdByCells = clCreateBuffer(this->_context,
            CL_MEM_READ_WRITE, this->_sizeGrid * sizeof(cl_int),
            NULL,
            &err);
    checkCLSuccess(err, "clCreateBuffer id by cell");

    this->_particleVelocity = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float) * 3, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer particle velocity");

    this->_particleProjection = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float) * 3, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer projection");

    this->_particleLambda = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_float), NULL, &err);
    checkCLSuccess(err, "clCreateBuffer projection");
    this->_particleNeighbors = clCreateBuffer(this->_context, CL_MEM_READ_WRITE, this->_nbParticle * sizeof(cl_int) * 200, NULL, &err);
    checkCLSuccess(err, "clCreateBuffer projection");
}


void    OpenCL::_bindVBO(GLuint vbo) {
    cl_int     err;

    this->_particle = clCreateFromGLBuffer(this->_context, CL_MEM_READ_WRITE, vbo, &err);
    checkCLSuccess(err, "clCreateFromGLBuffer");
}

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
}
