#ifndef MESH_HPP
# define MESH_HPP

# include <fstream>
# include <string>
# include <iostream>
# include <sstream>
# include <vector>

# include <glad/glad.h>
# include <GLFW/glfw3.h>

class Mesh {
    public:
        Mesh();
        std::vector<GLfloat>   *getVertices();
        std::vector<GLfloat>   *getNormal();
        std::vector<GLuint>    *getIndices();
        virtual ~Mesh();
    protected:
        std::vector<GLfloat>    *_vertices;
        std::vector<GLuint>     *_indicesVert;
        std::vector<GLuint>     *_indicesNorm;
        std::vector<GLfloat>    *_normal;
};

#endif
