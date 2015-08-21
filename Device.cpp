#include "Device.hpp"

Device::Device(cl_platform_id platformId)
    : _platformId(platformId) {
        checkCLSuccess(clGetDeviceIDs(this->_platformId,
                    CL_DEVICE_TYPE_ALL,
                    0,
                    NULL,
                    &(this->_nbDevices)),
                "clGetDeviceIDs");

        this->_deviceIds = (cl_device_id *)alloca(sizeof(cl_device_id) * this->_nbDevices);

        checkCLSuccess(clGetDeviceIDs(this->_platformId,
                    CL_DEVICE_TYPE_ALL,
                    this->_nbDevices,
                    this->_deviceIds,
                    NULL),
                "clGetDeviceIDs");
}

void    Device::displayInfoDevices() {
    size_t  valueSize = 0;
    char *  value;
    cl_uint maxComputeUnits;

    std::cout << "Nb devices: " << this->_nbDevices << std::endl;

    for (cl_uint j = 0; j < this->_nbDevices; j++) {
        // print device name
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_PROFILE, 10, NULL, &valueSize);
        std::cout << "Size: " << valueSize << std::endl;
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_PROFILE, valueSize, value, NULL);
        free(value);

        /**
        // print hardware device version
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize * sizeof(char *));
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_VERSION, valueSize * sizeof(char *), value, NULL);
        printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
        free(value);

        // print software driver version
        clGetDeviceInfo(this->_deviceIds[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(this->_deviceIds[j], CL_DRIVER_VERSION, valueSize, value, NULL);
        printf(" %d.%d Software version: %s\n", j+1, 2, value);
        free(value);

        // print c version supported by compiler for device
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
        printf(" %d.%d OpenCL C version: %s\n", j+1, 3, value);
        free(value);

        // print parallel compute units
        clGetDeviceInfo(this->_deviceIds[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                sizeof(maxComputeUnits), &maxComputeUnits, NULL);
        printf(" %d.%d Parallel compute units: %d\n\n", j+1, 4, maxComputeUnits);
        */
    }
}
