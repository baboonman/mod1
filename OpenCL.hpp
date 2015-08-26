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
# include <fstream>
# include "Platform.hpp"

class OpenCL {
    public:
        OpenCL( GLuint waterVBO, size_t sizeWaterVBO );
        virtual ~OpenCL( void );
        void            initOpenCL();
        void            executeKernel();
        void            release();
        std::string     *getKernel(std::string filename) const;
        static void     displayInformation();
    private:
        void                        _createContext();
        void                        _bindVBO();
        void                        _createProgram();
        void                        _getDeviceInfo();
        void                        _createCommandQueue( void );
        void                        _buildProgram();
        void                        _createKernel();
        void                        _setKernelArg();

        Platform                    _platform;
        cl_device_id                _device;
        cl_int                      _nbDevice;
        cl_context                  _context;
        cl_command_queue            _commandQueue;
        cl_uint                     _maxWorkingDimensions;
        size_t                      _localWorkSize;
        size_t                      _globalWorkSize;
        size_t                      _nbWorkGroup;
        cl_int                      _maxGid;
        cl_int                      _lineWidth;
        cl_program                  _program;
        cl_kernel                   _kernel;
        cl_mem                      _waterBuffer;
        cl_mem                      _atomicQuantity;

        GLuint                      _waterVBO;
        cl_int                      _sizeWaterVBO;
        size_t                      *_maxWorkItemSize;
};

#endif
