#ifndef MESH_MANAGER_HPP
# define MESH_MANAGER_HPP

/* Map height updates */
#define MAX_CIRCLE_SIZE (5.0f)
#define MAX_DISPLACEMENT (1.0f)
#define DISPLACEMENT_SIGN_LIMIT (0.3f)
#define MAX_ITER (200)
#define NUM_ITER_AT_A_TIME (1)

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <math.h>
# include <stdlib.h>
# include <iostream>

class MeshManager {
    public:
        MeshManager(unsigned int mapX, unsigned int mapY);
        void    makeMesh(GLuint program);
        void    updateMap(int numIter);
        void    updateMesh(void);
        void    initMap(void);
        void    updateView();
        void    generatePoint(void);
        void    generateIndices(void);
        GLuint  getNbIndices(void);
    private:
        GLuint  mesh;
        GLfloat *_mapVertices[3];
        GLuint  *_mapIndices;
        unsigned int _nbVertices;
        unsigned int _nbIndices;
        unsigned int _mapX;
        unsigned int _mapY;
        GLfloat      _rotX;
        GLfloat      _rotY;
        GLfloat      _rotZ;

        GLuint meshVbo[4];

        void    generateHeightmapCircle(float *centerX, float *centerY,
                float* size, float* displacement);
};

static GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.6f, 0.1f, 1.0f,
};
static GLfloat positionData[] = 
{
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
};

static GLuint indexes[] = 
{
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7, 
    0, 3, 7, 0, 7, 4, 
    1, 5, 6, 1, 6, 2, 
    0, 4, 5, 0, 5, 1, 
    2, 3, 7, 2, 7, 6
};

static const GLfloat bigCube[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};



#endif
