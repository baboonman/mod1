#include "WindowManager.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <stdlib.h>

int     main(int argc, char **argv) {
    (void) argv;
    WindowManager *windowManager;

    if (argc > 1) {
        OpenCL::displayInformation();
        return 0;
    }
    windowManager = new WindowManager(1000, 1080);
    windowManager->run();

    delete windowManager;

    return 0;
}
