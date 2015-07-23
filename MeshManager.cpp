#include "MeshManager.hpp"

MeshManager::MeshManager(unsigned int mapX, unsigned int mapY)
    : _mapX(mapX), _mapY(mapY) {

        this->_loadedMesh = new MapMesh(300, 300, 0.04);
        this->_mapManager = new MapManager("map", this->_loadedMesh);
        this->_mapManager->generateMountain();
 //       (*(this->_loadedMesh))(0, 5) = 3;
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

    this->_mapIndices = this->_loadedMesh->getIndices();
    this->_mapVertices = this->_loadedMesh->getVertices();
    index = new GLuint[this->_mapIndices->size()];
    vertice = new GLfloat[this->_mapVertices->size()];
    for (size_t i = 0; i < this->_mapVertices->size(); i++) {
        vertice[i] = (*this->_mapVertices)[i];
    }
    for (size_t i = 0; i < this->_mapIndices->size(); i++) {
        index[i] = (*this->_mapIndices)[i];
    }
    glGenVertexArrays(1, &(this->mesh));
    glGenBuffers(3, this->meshVbo);
    glBindVertexArray(this->mesh);
    /* Prepare the data for drawing through a buffer inidices */
    std::cout << "indice: " << this->_mapIndices << std::endl;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint) * this->_mapIndices->size(), index, GL_STATIC_DRAW);

    /* Prepare the attributes for rendering */
    std::cout << "vertex: " << this->_mapVertices<< std::endl;
    attrloc = glGetAttribLocation(program, "coord");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_mapVertices->size(), vertice, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    std::cout << "vertex: " << this->_mapVertices<< std::endl;
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
}
