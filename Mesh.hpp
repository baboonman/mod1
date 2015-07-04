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
        Mesh(std::string filename);
        std::vector<GLfloat>    *getVertices();
        std::vector<GLuint>    *getIndices();
    private:
        std::vector<GLfloat>    *_vertices;
        std::vector<GLuint>     *_indicesVert;
        std::vector<GLuint>     *_indicesNorm;
        std::vector<GLfloat>    *_normal;

        void    _addPoint(std::vector<std::string> &vector);
        void    _addNormal(std::vector<std::string> &vector);
        void    _addIndices(std::vector<std::string> &vector);
        void    _processLine(std::string);
        void    _split(std::vector<std::string> &vector, std::string line, std::string separator);
};

#endif
