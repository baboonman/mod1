#include "GenerateMatrix.hpp"

namespace GenerateMatrix {

    void setProjection( Matrix &matrix, GLfloat viewAngle, GLfloat aspectRatio, GLfloat zFar,GLfloat zNear) {
        float   f;
        matrix = MTX_INDENTITY;
        f = 1.0f / tanf(viewAngle / 2.0f);
        matrix[0]  = f / aspectRatio;
        matrix[5]  = f;
        matrix[10] = (zFar + zNear)/ (zNear - zFar);
        matrix[11] = -1.0f;
        matrix[14] = 2.0f * (zFar * zNear) / (zNear - zFar);
    }

    void setModelView(Matrix &matrix) {
        matrix = MTX_INDENTITY;
        matrix[12]  = -5.0f;
        matrix[13]  = -5.0f;
        matrix[14]  = -20.0f;
    }
}
