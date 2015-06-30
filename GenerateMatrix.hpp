#ifndef GENERATEMATRIX_HPP 
# define  GENERATEMATRIX_HPP

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cmath>
# include <iostream>

namespace Matrix {

    void setProjection( GLfloat *matrix, GLfloat viewAngle, GLfloat aspectRatio, GLfloat zFar,GLfloat zNear);
    void setModelView( GLfloat *matrix);
    void displayMatrix(GLfloat *matrix);

    const GLfloat  IDENTITY[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };
}

#endif
