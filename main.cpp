#include "WindowManager.hpp"
#include "Matrix.hpp"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    WindowManager *windowManager = new WindowManager(800, 600);
    delete windowManager;
    return 0;
}
