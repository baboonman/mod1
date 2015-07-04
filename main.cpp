#include "WindowManager.hpp"
#include "Matrix.hpp"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    WindowManager *windowManager = new WindowManager(1000, 1080);
    delete windowManager;
    return 0;
}
