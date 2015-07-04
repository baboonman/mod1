#include "MeshManager.hpp"

MeshManager::MeshManager(unsigned int mapX, unsigned int mapY)
    : _mapX(mapX), _mapY(mapY) {

        this->_loadedMesh = new Mesh("suzanne.obj");

    /*
    unsigned int    nbSquare = mapX * mapY;

    this->_nbVertices = (1 + this->_mapX) * (this->_mapY + 1);

    this->_mapVertices[0]  = new GLfloat[this->_nbVertices];
    this->_mapVertices[1]  = new GLfloat[this->_nbVertices];
    this->_mapVertices[2]  = new GLfloat[this->_nbVertices];
    std::cout << _nbVertices << std::endl;
    this->_nbIndices = nbSquare * 6;
    this->_mapIndices = new GLuint[this->_nbIndices];
    std::cout << "Nb §vertices: " << this->_nbVertices << " nb indices: " << this->_nbIndices << std::endl;
    this->generatePoint();
    this->generateIndices();
    */
}

void    MeshManager::generatePoint(void) {
    /*
    GLfloat     x;
    GLfloat     y;

    for (unsigned int i = 0; i < this->_nbVertices; i++) {
        x = i % (this->_mapX + 1);
        y = i / (this->_mapY + 1);
        this->_mapVertices[1][i] = x;
        this->_mapVertices[0][i] = y;
        this->_mapVertices[2][i] = 0.1f;
    }
    */
}

void    MeshManager::generateIndices(void) {
    /*
    unsigned int    i = 0;
    unsigned int    maxX = this->_mapX + 1;

    for (unsigned int y = 0; y < this->_mapY; y++) {
        for (unsigned int x = 1; x < maxX; x++) {

            this->_mapIndices[i++] = maxX * y + x;
            this->_mapIndices[i++] = maxX * y + x + maxX - 1;
            this->_mapIndices[i++] = maxX * y + (x - 1);

            this->_mapIndices[i++] = maxX * y + x;
            this->_mapIndices[i++] = maxX * y + x + maxX - 1;
            this->_mapIndices[i++] = maxX * y + x + maxX;
        }
    }
    */
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
    glGenBuffers(2, this->meshVbo);
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
    delete index;
    delete vertice;
}
