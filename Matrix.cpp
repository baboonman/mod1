#include "Matrix.hpp"

Matrix::Matrix(unsigned int sizeX, unsigned int sizeY)
    : _sizeX(sizeX), _sizeY(sizeY), _size(sizeX * sizeY) {
        this->_matrix = new GLfloat[this->_size];
}

Matrix::Matrix(unsigned int sizeX, unsigned int sizeY, const GLfloat *src)
    : _sizeX(sizeX), _sizeY(sizeY), _size(sizeX * sizeY) {
        this->_matrix = new GLfloat[this->_size];
        for (unsigned int i = 0; i < this->_size; i++) {
            this->_matrix[i] = src[i];
        }
}

void    Matrix::setMatrix(GLfloat *matrix) {
    for (unsigned int i = 0; i < this->_size; i++) {
        this->_matrix[i] = matrix[i];
    }
}

const GLfloat   *Matrix::toGLfloat(void) const {
    return const_cast<const GLfloat *>(this->_matrix);
}

GLfloat         &Matrix::operator ()(unsigned int x, unsigned int y) const {
    return this->_matrix[y * this->_sizeX + x];
}

std::ostream& operator<< (std::ostream &out, Matrix &matrix)
{
    out << std::setprecision(4);
    for (unsigned int y = 0; y < matrix._sizeY; y++) {
        out << "[";
        for (unsigned int x = 0; x < matrix._sizeX; x++) {
            out  << std::setw(6) << matrix(x, y);
            if (x != matrix._sizeX - 1) {
                std::cout << ",";
            }
        }
        out << " ]";
        out << std::endl;
    }
    return out;
}

Matrix          Matrix::operator* (const Matrix &m2) {
    Matrix      result(this->_sizeX, this->_sizeY);
    GLfloat     tmpTot;

    if (m2._sizeX != m2._sizeY || m2._sizeX != this->_sizeX || m2._sizeX != this->_sizeY) {
        std::cout << "Unable to make product of two different matrix size" << std::endl;
        return result;
    }

    for (unsigned int x = 0; x < this->_sizeX; x++) {
        for (unsigned int y = 0; y < this->_sizeY; y++) {
            tmpTot = 0;
            for (unsigned int i = 0; i < this->_sizeX; i++) {
                tmpTot += (*(this))(i, y) * m2(x, i);
            }
            result(x, y) = tmpTot;
        }
    }
    return result;
}

GLfloat&    Matrix::operator[] (const int n) {
    return this->_matrix[n];
}

void        Matrix::operator= (const Matrix &m) {
    for (unsigned int i = 0; i < this->_size; i++) {
        (*this)[i] = m._matrix[i];
    }
}
