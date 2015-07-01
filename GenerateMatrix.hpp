#ifndef GENERATEMATRIX_HPP 
# define  GENERATEMATRIX_HPP

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cmath>
# include <iostream>
# include "Matrix.hpp"

namespace GenerateMatrix {

    void    setProjection(Matrix &matrix, GLfloat viewAngle, GLfloat aspectRatio, GLfloat zFar,GLfloat zNear);
    void    setModelView(Matrix &matrix);
    void    setRotation(Matrix &matrix, GLfloat x, GLfloat y, GLfloat z);

    const GLfloat  IDENTITY[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };

    const Matrix    MTX_INDENTITY(4, 4, IDENTITY);
}

#endif
