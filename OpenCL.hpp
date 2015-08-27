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
# include "TaskParticleInGrid.hpp"
# include "TaskApplyForces.hpp"

class OpenCL {
    public:
        const size_t    X = 0;
        const size_t    Y = 1;
        const size_t    Z = 2;
        OpenCL(cl_int nbParticle, cl_float sizeGridCoef, cl_int maxParticlePerCell, cl_int gridX, cl_int gridY, cl_int gridZ);
        virtual ~OpenCL( void );
        void            initOpenCL();
        void            executeKernel();
        void            release();
        static void     displayInformation();
    private:
        void                        _createContext();
        void                        _bindBuffer();
        void                        _getDeviceInfo();
        void                        _createCommandQueue( void );
        void                        _setKernelArg();
        void                        _initTask();
        void                        _setStdArg(cl_kernel kernel);

        Platform                    _platform;
        cl_device_id                _device;
        cl_int                      _nbDevice;
        cl_context                  _context;
        cl_command_queue            _commandQueue;
        cl_uint                     _maxWorkingDimensions;
        cl_int                      _maxGid;
        cl_mem                      _particle;
        cl_mem                      _particleIdByCells;
        cl_mem                      _particleVelocity;
        cl_mem                      _particleProjection;

        cl_int                      _nbParticle;
        cl_int                      _gridSize[3];
        cl_float                    _sizeGridCoef;
        cl_int                      _maxParticlePerCell;
        TaskParticleInGrid          *_taskParticleInGrid = NULL;
        TaskApplyForces             *_taskApplyForces = NULL;

        size_t                      *_maxWorkItemSize;
};

#endif
