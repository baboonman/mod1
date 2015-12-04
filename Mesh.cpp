#include "Mesh.hpp"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

GLuint*							Mesh::getVBO( void ) { return this->_vbo; }
OpenGLMatrix*					Mesh::getModelMatrix( void ) { return &(this->_modelMatrix); }
void							Mesh::setModelMatrix( OpenGLMatrix const & newMatrix ) { this->_modelMatrix = newMatrix; }
std::vector<t_vec>				Mesh::getVertices() {return this->_vertices;}
std::vector<t_vec>				Mesh::getNormals() {return this->_normals;}
std::vector<GLuint>				Mesh::getIndices() {return this->_indices;}
