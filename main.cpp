#include "WindowManager.hpp"
#include "Matrix.hpp"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    Matrix      m1(4, 4, GenerateMatrix::IDENTITY);
    Matrix      m2(4, 4, GenerateMatrix::IDENTITY);
    Matrix      m3(4, 4, GenerateMatrix::IDENTITY);
 /*   m1(0, 2) = 1;
    m1(1, 2) = 0;
    m1(2, 2) = 0.5;
    m1(3, 2) = 0;

    */
    m1(2, 1) = 666;

    m2(2, 0) = 1;
    m2(2, 1) = 1;
    m2(2, 2) = 1;
    m2(2, 3) = 1;
    m3 = m1 * m2;
    std::cout << "m1: " << std::endl << m1 << std::endl;
    std::cout << "m2: " << std::endl << m2 << std::endl;
    std::cout << "m3: " << std::endl << m3 << std::endl;
    //WindowManager *windowManager = new WindowManager(800, 600);
    //delete windowManager;
    return 0;
}
