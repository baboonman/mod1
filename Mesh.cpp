#include "Mesh.hpp"

std::vector<GLfloat>    *Mesh::getVertices() {
    return (this->_vertices);
}

std::vector<GLuint>    *Mesh::getIndices() {
    return (this->_indicesVert);
}

std::vector<GLfloat>    *Mesh::getNormal() {
    return (this->_normal);
}

Mesh::Mesh() {
    this->_vertices = new std::vector<GLfloat>;
    this->_indicesVert = new std::vector<GLuint>;

    this->_normal = new std::vector<GLfloat>;
    this->_indicesNorm = new std::vector<GLuint>;
}

Mesh::~Mesh() {
    delete this->_vertices;
    delete this->_indicesVert;
    delete this->_normal;
    delete this->_indicesNorm;
}
