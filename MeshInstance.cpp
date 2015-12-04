#include "MeshInstance.hpp"

MeshInstance::MeshInstance()
{
}

MeshInstance::MeshInstance(Mesh & rhs, int nbInstances) : _nbInstances(nbInstances)
{
	this->loadMeshData(rhs);
}

MeshInstance::~MeshInstance()
{
}

void							MeshInstance::bindVBO(GLuint program)
{
	GLuint						attrloc;

	std::cout << "binding vbo to program: " << program << std::endl; 
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(4, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _normals.size(), &_normals[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * this->_nbInstances, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(program, "instancePosition");
	glEnableVertexAttribArray(attrloc);
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(attrloc, 1);

	glBindVertexArray(0);
}

void							MeshInstance::drawMesh() const
{
	glBindVertexArray(this->_vao);
	glDrawElementsInstanced(GL_TRIANGLES, this->_indices.size(), GL_UNSIGNED_INT, 0, this->_nbInstances);
	glBindVertexArray(0);
}

void							MeshInstance::updateMesh(float t)
{
}

void							MeshInstance::loadMeshData(Mesh & mesh)
{
	this->_vertices = mesh.getVertices();
	this->_normals = mesh.getNormals();
	this->_indices = mesh.getIndices();
}
