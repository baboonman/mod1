#include "MeshManager.hpp"

MeshManager::MeshManager(unsigned int mapX, unsigned int mapY)
    : _mapX(mapX), _mapY(mapY) {

    unsigned int    nbSquare = mapX * mapY;

    this->_nbVertices = (1 + this->_mapX) * (this->_mapY + 1);

    this->_mapVertices[0]  = new GLfloat[this->_nbVertices];
    this->_mapVertices[1]  = new GLfloat[this->_nbVertices];
    this->_mapVertices[2]  = new GLfloat[this->_nbVertices];
    std::cout << _nbVertices << std::endl;
    this->_nbIndices = nbSquare * 6;
    this->_mapIndices = new GLuint[this->_nbIndices];
    std::cout << "Nb vertices: " << this->_nbVertices << " nb indices: " << this->_nbIndices << std::endl;
    this->generatePoint();
    this->generateIndices();
}

void    MeshManager::generatePoint(void) {
    GLfloat     x;
    GLfloat     y;

    for (unsigned int i = 0; i < this->_nbVertices; i++) {
        x = i % (this->_mapX + 1);
        y = i / (this->_mapY + 1);
        this->_mapVertices[1][i] = x;
        this->_mapVertices[0][i] = y;
        this->_mapVertices[2][i] = 0.1f;
    }
}

void    MeshManager::generateIndices(void) {
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
}

GLuint    MeshManager::getNbIndices(void) {
    return this->_nbIndices;
}

void    MeshManager::makeMesh(GLuint program) {

    GLuint attrloc;

    glGenVertexArrays(1, &(this->mesh));
    glGenBuffers(4, this->meshVbo);
    glBindVertexArray(this->mesh);
    /* Prepare the data for drawing through a buffer inidices */
    std::cout << "nb indices: " << this->_nbIndices << std::endl;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_nbIndices * sizeof(GLuint), this->_mapIndices, GL_STATIC_DRAW);

    /* Prepare the attributes for rendering */
    attrloc = glGetAttribLocation(program, "x");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_nbVertices, this->_mapVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 1, GL_FLOAT, GL_FALSE, 0, 0);

    attrloc = glGetAttribLocation(program, "y");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_nbVertices, this->_mapVertices[1], GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 1, GL_FLOAT, GL_FALSE, 0, 0);

    attrloc = glGetAttribLocation(program, "z");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->_nbVertices, this->_mapVertices[2], GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 1, GL_FLOAT, GL_FALSE, 0, 0);
}
