#ifndef OPENCL_HPP
# define OPENCL_HPP

# define GLFW_EXPOSE_NATIVE_COCOA
# define GLFW_EXPOSE_NATIVE_NSGL

# include <string>
# include <iostream>
# include <OpenCL/cl.h>
# include <OpenCL/cl_gl.h>
# include <OpenCL/cl_gl_ext.h>
# include <OpenGL/OpenGL.h>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <GLFW/glfw3native.h>
# include "Platform.hpp"

class OpenCL {
    public:
        OpenCL( int platformId, int deviceId, GLuint waterVBO );
        void            initOpenCL();
        void            _getDeviceInfo();
        static void     displayInformation();
    private:
        void                        _createContext();
        void                        _bindVBO();
        Platform                    _platform;
        void                        _createCommandQueue( void );
        Device                      *_currentDevice;
        cl_context_properties       _contextProp[3];
        int                         _platformId;
        int                         _deviceId;
        GLuint                      _waterVBO;
        cl_device_id                _device;
        cl_int                      _nbDevice;
        cl_context                  _context;
        cl_command_queue            _commandQueue;
};

#endif
