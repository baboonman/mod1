#ifndef MAPMESH_HPP
# define MAPMESH_HPP

# include <fstream>
# include <string>
# include <iostream>
# include <sstream>
# include <vector>

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include "Mesh.hpp"
# include "Vector.hpp"

class MapMesh : public Mesh {
    public:
        const size_t    X = 0;
        const size_t    Y = 1;
        const size_t    Z = 2;
        MapMesh(unsigned int sizeX, unsigned int sizeY, float step);
        GLfloat         &operator() (unsigned int x, unsigned int y) const;
        unsigned int    getSizeX();
        unsigned int    getSizeY();
        void            calcNormal();
    private:
        void            _generatePoint();

        unsigned int    _sizeX;
        unsigned int    _sizeY;
        float           _step;
};

#endif
