#include "GenerateMatrix.hpp"

namespace GenerateMatrix {

    void setProjection( Matrix &matrix, GLfloat viewAngle, GLfloat aspectRatio, GLfloat zFar,GLfloat zNear) {
        float   f;
        matrix = MTX_INDENTITY;
        f = 1.0f / tanf(viewAngle / 2.0f);
        matrix[0]  = f / aspectRatio;
        matrix[5]  = f;
        matrix[10] = (zFar + zNear) / (zNear - zFar);
        matrix[11] = -1.0f;
        matrix[14] = 2.0f * (zFar * zNear) / (zNear - zFar);
        matrix[15] = 0.0f;

        /*
        GLfloat top = zNear * tanf((M_PI / 180) * 90.0f / 2.0);
        GLfloat bottom = -top;
        GLfloat right = top * aspectRatio;
        GLfloat left = -right;

        matrix = MTX_INDENTITY;
        matrix(0, 0) = (2 * zNear) / (right - left);
        matrix(1, 1) = (2 * zNear) / (top - bottom);
        matrix(2, 2) = -(zFar + zNear) / (zFar - zNear);
        matrix(3, 3) = 0.0f;
        matrix(0, 2) = (right + left) / (right - left);
        matrix(1, 2) = (top + bottom) / (top - bottom);
        matrix(2, 3) = (2 * zFar * zNear) / (zFar - zNear);
        */
    }

    void setModelView(Matrix &matrix) {
        matrix = MTX_INDENTITY;
        matrix(3, 0)  = 0.0f;
        matrix(3, 1)  = 0.0f;
        matrix(3, 2) = -5.0f;
    }

    void    setRotation(Matrix &matrix, GLfloat x, GLfloat y, GLfloat z) {
        GLfloat sinX = sin(x);
        GLfloat sinY= sin(y);
        GLfloat sinZ = sin(z);

        GLfloat cosX = cos(x);
        GLfloat cosY= cos(y);
        GLfloat cosZ = cos(z);

        matrix(0, 0) = cosY * cosZ;
        matrix(0, 1) = cosX * sinZ + sinX * sinY * cosZ;
        matrix(0, 2) = sinX * sinZ - cosX * sinY * cosZ;
        matrix(0, 3) = 0;

        matrix(1, 0) = -cosY * sinZ;
        matrix(1, 1) = cosX * cosZ - sinX * sinY * sinZ;
        matrix(1, 2) = cosX * sinY * sinZ + sinX * cosZ;
        matrix(1, 3) = 0;

        matrix(2, 0) = sinY;
        matrix(2, 1) = -sinX * cosY;
        matrix(2, 2) = cosX * cosY;
        matrix(2, 3) = 0;

        matrix(3, 0) = 0;
        matrix(3, 1) = 0;
        matrix(3, 2) = 0;
        matrix(3, 3) = 1;
    }
}
