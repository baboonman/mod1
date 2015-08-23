#include "MeshManager.hpp"

MeshManager::MeshManager(unsigned int mapX, unsigned int mapY)
    : _mapX(mapX), _mapY(mapY) {

        this->_loadedMesh = new MapMesh(300, 300, 0.04);
        this->_mapManager = new MapManager("map", this->_loadedMesh);
        this->_mapManager->generateMountain();
}

MeshManager::~MeshManager() {
    delete this->_loadedMesh;
    delete this->_mapManager;

}

GLuint    MeshManager::getNbIndices(void) {
    return this->_loadedMesh->getIndices()->size();
}

void    MeshManager::makeMesh(GLuint program) {

    GLuint attrloc;
    GLuint  *index;
    GLfloat *vertice;
    GLfloat *waterZ;

    this->_mapIndices = this->_loadedMesh->getIndices();
    this->_mapVertices = this->_loadedMesh->getVertices();

    this->_waterZ = new std::vector<GLfloat>;

    index = new GLuint[this->_mapIndices->size()];
    vertice = new GLfloat[this->_mapVertices->size()];

    for (size_t i = 0; i < this->_mapVertices->size(); i++) {
        vertice[i] = (*this->_mapVertices)[i];
        if ((i  + 1) % 3 == 0) {
            this->_waterZ->push_back(vertice[i]);
        }
    }

    waterZ = new GLfloat[this->_waterZ->size()];

    for (size_t i = 0; i < this->_waterZ->size(); i++) {
        waterZ[i] = (*this->_waterZ)[i];
    }

    for (size_t i = 0; i < this->_mapIndices->size(); i++) {
        index[i] = (*this->_mapIndices)[i];
    }

    glGenVertexArrays(1, &(this->mesh));
    glGenBuffers(4, this->meshVbo);
    glBindVertexArray(this->mesh);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint) * this->_mapIndices->size(), index, GL_STATIC_DRAW);

    attrloc = glGetAttribLocation(program, "coord");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_mapVertices->size(), vertice, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    attrloc = glGetAttribLocation(program, "waterz");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_waterZ->size(), waterZ, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 1, GL_FLOAT, GL_FALSE, 0, 0);

    attrloc = glGetAttribLocation(program, "normal");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[2]);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(GLfloat) * this->_loadedMesh->getNormal()->size(),
            this->_loadedMesh->getNormal()->data(),
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    delete index;
    delete vertice;
    delete waterZ;
}

GLuint      MeshManager::getWaterVBO() {
    return this->meshVbo[3];
}
