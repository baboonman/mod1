#include "MapMesh.hpp"

MapMesh::MapMesh(unsigned int sizeX, unsigned int sizeY, float step) 
    : Mesh(), _sizeX(sizeX), _sizeY(sizeY), _step(step) {
	
	std::vector<GLfloat>	mapVal;
	MapManager map("map", 300, 300, 0.4);
    this->_generatePoint();
	mapVal = map.generateMountain();
	this->_transMap(mapVal);
	this->calcNormal();
}

void	MapMesh::_transMap(std::vector<GLfloat> map)
{
	unsigned int		x, y;

    for (unsigned int i = 0; i < map.size(); i++) {
			x = i % this->_sizeX - 1;
			y = i / this->_sizeX;
//    		(*this->_vertices)[i] = map[i];
    		(*this->_vertices)[x * this->_sizeY * 3 + y * 3 + MapMesh::Z] = map[i];
        }
}


void    MapMesh::calcNormal() {
    Vector  v1;
    Vector  v2;
    Vector  normal;
    GLfloat *p1;
    GLfloat *p2;
    GLfloat *p3;
    GLfloat *rawVertices;

    rawVertices = this->_vertices->data();

    for (unsigned int i = 0; i < this->_vertices->size(); i += 9) {
        p1 = rawVertices + i;
        p2 = rawVertices + i + 3;
        p3 = rawVertices + i + 6;
        v1.x = p2[0] - p1[0];
        v1.y = p2[1] - p1[1];
        v1.z = p2[2] - p1[2];
        v2.x = p3[0] - p1[0];
        v2.y = p3[1] - p1[1];
        v2.z = p3[2] - p1[2];
        v1 = v1.getUnit();
        v2 = v2.getUnit();
        normal = v1 * v2;
        for (unsigned int j = 0; j < 3; ++j) {
            this->_normal->push_back(normal.x);
            this->_normal->push_back(normal.y);
            this->_normal->push_back(normal.z);
        }
    }
}

void    MapMesh::_generatePoint() {
    for (unsigned int y = 0; y < this->_sizeY; y++) {
        for (unsigned int x = 0; x < this->_sizeX; x++) {
            this->_vertices->push_back(static_cast<GLfloat>(x) * this->_step);
            this->_vertices->push_back(static_cast<GLfloat>(y) * this->_step);
            this->_vertices->push_back(0.0f);
        }
    }

    for (unsigned int y = 0; y < this->_sizeY - 1; y++) {
        for (unsigned int x = 1; x < this->_sizeX; x++) {
            this->_indicesVert->push_back(this->_sizeX * y + x);
            this->_indicesVert->push_back(this->_sizeX * y + (x - 1));
            this->_indicesVert->push_back(this->_sizeX * (y + 1) + (x - 1));
        }
    }

    for (unsigned int y = 0; y < this->_sizeY - 1; y++) {
        for (unsigned int x = 1; x < this->_sizeX; x++) {
            this->_indicesVert->push_back(this->_sizeX * y + x);
            this->_indicesVert->push_back(this->_sizeX * (y + 1) + (x - 1));
            this->_indicesVert->push_back(this->_sizeX * (y + 1) + x);
        }
    }
}

GLfloat         &MapMesh::operator ()(unsigned int x, unsigned int y) const {
    return (*this->_vertices)[x * this->_sizeY * 3 + y * 3 + MapMesh::Z];
}

unsigned int    MapMesh::getSizeX() {
    return this->_sizeX;
}
unsigned int    MapMesh::getSizeY() {
    return this->_sizeY;
}
