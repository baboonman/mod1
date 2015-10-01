#include "OpenGLScene.hpp"

OpenGLScene::OpenGLScene() : _progID(0)
{
}

int					OpenGLScene::addShaderProg(std::string VSFile, std::string FSFile)
{
	OpenGLShader	*shader = new OpenGLShader;

	shader->addShader(GL_VERTEX_SHADER, VSFile);
	shader->addShader(GL_FRAGMENT_SHADER , FSFile);
	
	if (! shader->createProgram())
		return (-1);

	this->_progs.push_back(shader);
	this->_progID++;
	std::cout << "Program created: " << this->_progs[this->_progID - 1]->getProgram() << std::endl;
	return (this->_progID - 1);
}

int					OpenGLScene::addMesh(int type, int progID)
{
	Mesh			*mesh;
	if (type == MESH_WATER || type == MESH_MOUNT)
	{
		mesh = new Mesh;
		mesh->initMeshIndices(this->_progs[progID]->getProgram());
		this->_meshes[progID].push_back(mesh);
		return (progID);
	}
	return (-1);
}

int					OpenGLScene::addMesh(int type, std::string filename, int progID)
{
	Mesh			*mesh;
	if (type == MESH_OBJ)
	{
		mesh = new Mesh(filename);
		mesh->initMeshIndices(this->_progs[progID]->getProgram());
		this->_meshes[progID].push_back(mesh);
		return (progID);
	}
	return (-1);
}

int					OpenGLScene::addLight(int type)
{
}

int					OpenGLScene::addParticleSystem(int type, int nb, int progID)
{
}

int					OpenGLScene::addParticleSystem(int type, std::string filename, int nb, int progID)
{
}

int					OpenGLScene::drawScene(OpenGLMatrix view, OpenGLMatrix project, float t)
{
	GLuint			progID;

	for (const auto& itMap : this->_meshes)
	{
		progID = this->_progs[itMap.first]->getProgram();
		glUseProgram(progID);
		for (const auto& itVector : itMap.second)
		{
			this->addMatricesToProgram(progID, itVector->getModelMatrix(), view, project, 50);
			itVector->updateTerrain(t);
			itVector->drawMesh();
		}
		glUseProgram(0);
	}
}

void					OpenGLScene::addMatricesToProgram(GLuint progID, OpenGLMatrix model, OpenGLMatrix view, OpenGLMatrix project, float h)
{
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;
	GLint			uloc_H;

    uloc_M = glGetUniformLocation(progID, "M");
    uloc_V = glGetUniformLocation(progID, "V");
    uloc_P = glGetUniformLocation(progID, "P");
    uloc_H = glGetUniformLocation(progID, "H");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, project.getMatrixArray());
    glUniform1f(uloc_H, h);
}
