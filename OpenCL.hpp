#ifndef OPENCL_HPP
# define OPENCL_HPP

# include <string>
# include <iostream>
# include <OpenCL/cl.h>
# include "Platform.hpp"

class OpenCL {
    public:
        OpenCL( void );
        void    displayInformation();
    private:
        Platform    platform;
};

#endif
