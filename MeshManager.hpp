#ifndef MESH_MANAGER_HPP
# define MESH_MANAGER_HPP

/* Map height updates */
#define MAX_CIRCLE_SIZE (5.0f)
#define MAX_DISPLACEMENT (1.0f)
#define DISPLACEMENT_SIGN_LIMIT (0.3f)
#define MAX_ITER (200)
#define NUM_ITER_AT_A_TIME (1)

/* Map general information */
#define MAP_SIZE (10.0f)
#define MAP_NUM_VERTICES (80)
#define MAP_NUM_TOTAL_VERTICES (MAP_NUM_VERTICES*MAP_NUM_VERTICES)
#define MAP_NUM_LINES (3* (MAP_NUM_VERTICES - 1) * (MAP_NUM_VERTICES - 1) + \
               2 * (MAP_NUM_VERTICES - 1))

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <math.h>
# include <stdlib.h>

class MeshManager {
    public:
        void    makeMesh(GLuint program);
        void    updateMap(int numIter);
        void    updateMesh(void);
        void    initMap(void);
    private:
        GLuint  mesh;
        GLuint  meshVbo[4];
        GLfloat mapVertices[3][MAP_NUM_TOTAL_VERTICES];
        GLuint  mapLineIndices[2 * MAP_NUM_LINES];

        void    generateHeightmapCircle(float *centerX, float *centerY,
                float* size, float* displacement);
};

#endif
