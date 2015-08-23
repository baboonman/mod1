#include "OpenCL.hpp"

OpenCL::OpenCL(GLuint waterVBO, size_t sizeWaterVBO) 
: _waterVBO(waterVBO), _sizeWaterVBO(sizeWaterVBO), _maxWorkItemSize(NULL)
{
}

void    OpenCL::initOpenCL() {
    this->_createContext();
    this->_getDeviceInfo();
    this->_bindVBO();
    this->_createCommandQueue();
    this->_createProgram();
    this->_buildProgram();
    this->_createKernel();
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

std::string     *OpenCL::getKernel(std::string filename) const {
    std::ifstream ifs(filename);
    std::string *content;
        
    content = new std::string( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()) );
    return content;
}

void    OpenCL::_createProgram() {
    const char        *src;
    std::string *srcStr;

    srcStr = this->getKernel("kernel/waterSimulation.cl");
    src = srcStr->c_str();

    this->_program = clCreateProgramWithSource(this->_context, 1, &src, NULL, NULL);
    if (this->_program == NULL) {
        std::cerr << "Program creation fail" << std::endl;
        throw new OpenCLException;
    }
}

void    OpenCL::_createKernel() {
    int     err;

    this->_kernel = clCreateKernel(this->_program, "waterSimulation", &err);
    checkCLSuccess(err, "clCreateKernel");
    err = clGetKernelWorkGroupInfo(
        this->_kernel,
        this->_device,
        CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(this->_localWorkSize),
        &(this->_localWorkSize),
        NULL);

    checkCLSuccess(err, "clGetKernelWorkGroupInfo");
    this->_nbWorkGroup = (this->_sizeWaterVBO / this->_localWorkSize + 1);
    std::cout << "nb work group: " << this->_nbWorkGroup << std::endl;
    this->_globalWorkSize = this->_nbWorkGroup * this->_localWorkSize;
    std::cout << "Local work size: " << this->_localWorkSize << std::endl;
    std::cout << "global work size: " << this->_globalWorkSize << std::endl;
}

void    OpenCL::_setKernelArg() {
    checkCLSuccess(clSetKernelArg(this->_kernel, 0, sizeof(cl_mem), &this->_waterBuffer),
            "clSetKernelArg");
    checkCLSuccess(clSetKernelArg(this->_kernel, 1, sizeof(cl_int), &this->_sizeWaterVBO),
            "clSetKernelArg");
}

void    OpenCL::executeKernel() {
    int     err;


    err = clEnqueueAcquireGLObjects(
        this->_commandQueue,
        1,
        &this->_waterBuffer,
        0,
        NULL,
        NULL),
    checkCLSuccess(err, "clEnqueueAcquireGLObjects");
    err = clEnqueueNDRangeKernel(
            this->_commandQueue,
            this->_kernel,
            1,
            NULL,
            &this->_globalWorkSize,
            &this->_localWorkSize,
            0,
            NULL,
            NULL);
    checkCLSuccess(err, "clEnqueueNDRangeKernel");
    clFinish(this->_commandQueue);
    err = clEnqueueReleaseGLObjects(
            this->_commandQueue,
            1,
            &this->_waterBuffer,
            0,
            NULL,
            NULL);

    checkCLSuccess(err, "clEnqueueReleaseGLObjects");
}

void    OpenCL::_buildProgram() {
    int     err;

    err = clBuildProgram(this->_program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char    buildlog[16384];
        clGetProgramBuildInfo(this->_program, this->_device, CL_PROGRAM_BUILD_LOG, sizeof(buildlog), buildlog, NULL);
        std::cerr << "Error on compilation: " << std::endl;
        std::cerr << buildlog << std::endl;
        throw new OpenCLException();
    }
}

void    OpenCL::_bindVBO() {
    cl_int     err;

    this->_waterBuffer = clCreateFromGLBuffer(this->_context, CL_MEM_READ_WRITE, this->_waterVBO, &err);
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
    if (this->_maxWorkItemSize != NULL) {
     //   delete[] this->_maxWorkItemSize;
    }
}
