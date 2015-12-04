#include "MeshGenerator.hpp"

MeshGenerator::MeshGenerator()
{
	std::cout << "Creating Mesh Geometry" << std::endl;
	this->loadMeshGeometry();
}

MeshGenerator::~MeshGenerator()
{
}

void							MeshGenerator::bindVBO(GLuint program)
{
	GLuint						attrloc;
	GLenum						usage = GL_STATIC_DRAW;

	std::cout << "binding vbo to program: " << program << std::endl; 
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(3, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * this->_vertices.size(), &this->_vertices[0], usage);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * this->_normals.size(), &this->_normals[0], usage);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->_indices.size(), &this->_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void							MeshGenerator::drawMesh() const
{
	glBindVertexArray(this->_vao);
	glDrawElements(GL_TRIANGLES, this->_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void							MeshGenerator::updateMesh(float t)
{
}

void							MeshGenerator::loadMeshGeometry()
{
	HeightmapGenerator			gen;

	gen.exportMeshData(this->_vertices, this->_normals, this->_indices);
}
