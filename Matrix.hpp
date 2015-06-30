#ifndef MATRIX_HPP
# define MATRIX_HPP

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <iostream>
# include <iomanip>

class Matrix {
    public:
        Matrix(unsigned int sizeX, unsigned int sizeY);
        Matrix(unsigned int sizeX, unsigned int sizeY, const GLfloat *src);
        void            setMatrix(GLfloat *matrix);
        const GLfloat   *toGLfloat(void) const;
        friend std::ostream& operator<< (std::ostream &out, Matrix &matrix);
        Matrix          operator* (const Matrix &m2);
        GLfloat         &operator() (unsigned int x, unsigned int y) const;
        GLfloat         &operator[] (const int n);
        void            operator= (const Matrix &m);
    private:
        GLfloat         *_matrix;
        const unsigned int    _sizeX;
        const unsigned int    _sizeY;
        const unsigned int    _size;
};


#endif
