#include "GenerateMatrix.hpp"

namespace Matrix {
    namespace  {
        void copyMatrix(const GLfloat *src, GLfloat *dest) {
            for (int i = 0; i < 16; i++) {
                dest[i] = src[i];
            }
        }
    }

    void setProjection( GLfloat *matrix, GLfloat viewAngle, GLfloat aspectRatio, GLfloat zFar,GLfloat zNear) {
        float   f;
        copyMatrix(IDENTITY, matrix);
        f = 1.0f / tanf(viewAngle / 2.0f);
        matrix[0]  = f / aspectRatio;
        matrix[5]  = f;
        matrix[10] = (zFar + zNear)/ (zNear - zFar);
        matrix[11] = -1.0f;
        matrix[14] = 2.0f * (zFar * zNear) / (zNear - zFar);
    }

    void setModelView( GLfloat *matrix) {
        copyMatrix(IDENTITY, matrix);
        matrix[12]  = -5.0f;
        matrix[13]  = -5.0f;
        matrix[14]  = -20.0f;
    }

    void displayMatrix(GLfloat *matrix) {
        for (int i = 0; i < 16; i++) {
            if (i % 4 == 0)
                std::cout << std::endl;
            std::cout << matrix[i] << ",";
        }
        std::cout << std::endl;
    }
}
