#ifndef PLATFORM_HPP
# define PLATFORM_HPP

# include <OpenCL/cl.h>
# include <string>
# include <iostream>
# include "clUtil.hpp"
# include "Device.hpp"

class Platform {
    public:
        Platform( void );
        void    displayInfoPlatforms();
    private:
        cl_uint         numPlatforms;
        cl_platform_id  *platformIds;
        void            queryInfoPlatform(cl_platform_info query, int idPlatform, std::string paramName);
};

#endif
