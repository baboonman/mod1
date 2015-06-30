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

void    MeshManager::updateMap(int numIter) {
    while(numIter)
    {
        /* center of the circle */
        float centerX;
        float centerZ;
        float circleSize;
        float disp;
        size_t ii;

        this->generateHeightmapCircle(&centerX, &centerZ, &circleSize, &disp);
        disp = disp / 2.0f;
        for (ii = 0u ; ii < MAP_NUM_TOTAL_VERTICES ; ++ii)
        {
            GLfloat dx = centerX - mapVertices[0][ii];
            GLfloat dz = centerZ - mapVertices[2][ii];
            GLfloat pd = (2.0f * sqrtf((dx * dx) + (dz * dz))) / circleSize;
            if (fabs(pd) <= 1.0f)
            {
                /* tx,tz is within the circle */
                GLfloat newHeight = disp + (float) (cos(pd*3.14f)*disp);
                mapVertices[1][ii] += newHeight;
            }
        }
        --numIter;
    }
}

void    MeshManager::generateHeightmapCircle(float *centerX, float *centerY,
        float* size, float* displacement)
{
    float sign;
    /* random value for element in between [0-1.0] */
    *centerX = (MAP_SIZE * rand()) / (1.0f * RAND_MAX);
    *centerY = (MAP_SIZE * rand()) / (1.0f * RAND_MAX);
    *size = (MAX_CIRCLE_SIZE * rand()) / (1.0f * RAND_MAX);
    sign = (1.0f * rand()) / (1.0f * RAND_MAX);
    sign = (sign < DISPLACEMENT_SIGN_LIMIT) ? -1.0f : 1.0f;
    *displacement = (sign * (MAX_DISPLACEMENT * rand())) / (1.0f * RAND_MAX);
}

void    MeshManager::updateMesh(void)
{
  //  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * MAP_NUM_TOTAL_VERTICES, &(this->mapVertices[1][0]));
}

void    MeshManager::initMap( void ) {
    int i;
    int j;
    int k;
    GLfloat step = MAP_SIZE / (MAP_NUM_VERTICES - 1);
    GLfloat x = 0.0f;
    GLfloat z = 0.0f;
    /* Create a flat grid */
    k = 0;
    for (i = 0 ; i < MAP_NUM_VERTICES ; ++i)
    {
        for (j = 0 ; j < MAP_NUM_VERTICES ; ++j)
        {
            this->mapVertices[0][k] = x;
            this->mapVertices[1][k] = 0.0f;
            this->mapVertices[2][k] = z;
            z += step;
            ++k;
        }
        x += step;
        z = 0.0f;
    }
#if DEBUG_ENABLED
    for (i = 0 ; i < MAP_NUM_TOTAL_VERTICES ; ++i)
    {
        printf ("Vertice %d (%f, %f, %f)\n",
                i, this->mapVertices[0][i], this->mapVertices[1][i], this->mapVertices[2][i]);

    }
#endif
    /* create indices */
    /* line fan based on i
     * i+1
     * |  / i + n + 1
     * | /
     * |/
     * i --- i + n
     */

    /* close the top of the square */
    k = 0;
    for (i = 0 ; i < MAP_NUM_VERTICES  -1 ; ++i)
    {
        this->mapLineIndices[k++] = (i + 1) * MAP_NUM_VERTICES -1;
        this->mapLineIndices[k++] = (i + 2) * MAP_NUM_VERTICES -1;
    }
    /* close the right of the square */
    for (i = 0 ; i < MAP_NUM_VERTICES -1 ; ++i)
    {
        this->mapLineIndices[k++] = (MAP_NUM_VERTICES - 1) * MAP_NUM_VERTICES + i;
        this->mapLineIndices[k++] = (MAP_NUM_VERTICES - 1) * MAP_NUM_VERTICES + i + 1;
    }

    for (i = 0 ; i < (MAP_NUM_VERTICES - 1) ; ++i)
    {
        for (j = 0 ; j < (MAP_NUM_VERTICES - 1) ; ++j)
        {
            int ref = i * (MAP_NUM_VERTICES) + j;
            this->mapLineIndices[k++] = ref;
            this->mapLineIndices[k++] = ref + 1;

            this->mapLineIndices[k++] = ref;
            this->mapLineIndices[k++] = ref + MAP_NUM_VERTICES;

            this->mapLineIndices[k++] = ref;
            this->mapLineIndices[k++] = ref + MAP_NUM_VERTICES + 1;
        }
    }

#ifdef DEBUG_ENABLED
    for (k = 0 ; k < 2 * MAP_NUM_LINES ; k += 2)
    {
        int beg, end;
        beg = this->mapLineIndices[k];
        end = this->mapLineIndices[k+1];
        printf ("Line %d: %d -> %d (%f, %f, %f) -> (%f, %f, %f)\n",
                k / 2, beg, end,
                this->mapVertices[0][beg], this->mapVertices[1][beg], this->mapVertices[2][beg],
                this->mapVertices[0][end], this->mapVertices[1][end], this->mapVertices[2][end]);
    }
#endif
}

