#include "WindowManager.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "OpenCL.hpp"


int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    OpenCL  openCl;

    openCl.displayInformation();
//    WindowManager *windowManager = new WindowManager(1000, 1080);
//    delete windowManager;
    return 0;
}
