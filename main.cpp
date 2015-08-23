#include "OpenCL.hpp"
#include "WindowManager.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <stdlib.h>

int     main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    OpenCL          *openCl;
    WindowManager *windowManager;

    if (argc < 3) {
        OpenCL::displayInformation();
        return 0;
    }

    windowManager = new WindowManager(1000, 1080);
    openCl = new OpenCL(atoi(argv[1]), atoi(argv[2]), windowManager->getWaterVBO());
    openCl->initOpenCL();
    windowManager->run();
    delete windowManager;

    return 0;
}
