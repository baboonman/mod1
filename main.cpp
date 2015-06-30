#include "WindowManager.hpp"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    WindowManager *windowManager = new WindowManager(800, 800);
    free(windowManager);
    return 0;
}
