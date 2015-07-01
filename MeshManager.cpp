#include "MeshManager.hpp"

void    MeshManager::makeMesh(GLuint program) {

    GLuint attrloc;

  //  glEnable(GL_DEPTH_TEST);
  //  glDepthFunc(GL_LESS);
    glGenVertexArrays(1, &(this->mesh));
    glGenBuffers(4, this->meshVbo);
    glBindVertexArray(this->mesh);
    /* Prepare the data for drawing through a buffer inidices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLfloat), indexes, GL_STATIC_DRAW);

    /* Prepare the attributes for rendering */
    attrloc = glGetAttribLocation(program, "coord");
    glBindBuffer(GL_ARRAY_BUFFER, this->meshVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, positionData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attrloc);
    glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
