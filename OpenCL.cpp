#include "OpenCL.hpp"

OpenCL::OpenCL( int platformId, int deviceId, GLuint waterVBO) 
: _platformId(platformId), _deviceId(deviceId), _waterVBO(waterVBO)
{
}

void    OpenCL::initOpenCL() {
    this->_createContext();
    this->_getDeviceInfo();
    this->_bindVBO();
}

void    OpenCL::_createContext() {
    cl_int      err;
    size_t      size;

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
}

void    OpenCL::_bindVBO() {
    cl_int     err;

    clCreateFromGLBuffer(this->_context, CL_MEM_READ_WRITE, this->_waterVBO, &err);
    checkCLSuccess(err, "clCreateFromGLBuffer");
}

void    OpenCL::_createCommandQueue( void ) {
//    this->_commandQueue = clCreateCommandQueue(this->_context, 
}

void    OpenCL::displayInformation( void ) {
    Platform    platform;

    platform.displayInfoPlatforms();
}
