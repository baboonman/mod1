#include "OpenCL.hpp"

OpenCL::OpenCL( void ) {
}


void    OpenCL::displayInformation( void ) {
    this->platform.displayInfoPlatforms();
}
