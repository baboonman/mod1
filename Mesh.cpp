#include "Mesh.hpp"

std::vector<GLfloat>    *Mesh::getVertices() {
    return (this->_vertices);
}

std::vector<GLuint>    *Mesh::getIndices() {
    return (this->_indicesVert);
}

void    Mesh::_addPoint(std::vector<std::string> &vector) {
    this->_vertices->push_back(std::atof(vector[1].c_str()));
    this->_vertices->push_back(std::atof(vector[2].c_str()));
    this->_vertices->push_back(std::atof(vector[3].c_str()));
}

void    Mesh::_addNormal(std::vector<std::string> &vector) {
    this->_normal->push_back(std::atof(vector[1].c_str()));
    this->_normal->push_back(std::atof(vector[2].c_str()));
    this->_normal->push_back(std::atof(vector[3].c_str()));
}


void    Mesh::_addIndices(std::vector<std::string> &vector) {
    std::vector<std::string> i1;
    std::vector<std::string> i2;
    std::vector<std::string> i3;

    this->_split(i1, vector[1], "/");
    this->_split(i2, vector[2], "/");
    this->_split(i3, vector[3], "/");
    this->_indicesVert->push_back(std::atoi(i1[0].c_str()) - 1);
    this->_indicesVert->push_back(std::atoi(i2[0].c_str()) - 1);
    this->_indicesVert->push_back(std::atoi(i3[0].c_str()) - 1);
}

void    Mesh::_split(std::vector<std::string> &vector, std::string line, std::string separator) {
    size_t      previous = 0;
    size_t      i;

    for (i = 0; i < line.size(); i++) {
        if (line[i] == separator[0]) {
            vector.push_back(line.substr(previous, i));
            previous = i;
        }
    }
    vector.push_back(line.substr(previous, i));
}

void    Mesh::_processLine(std::string line) {

    std::vector<std::string>    vector;

    this->_split(vector, line, " ");

  //  if (vector.size() < 4) {
  //      return ;
  //  }

    if (vector[0] == "v") {
        this->_addPoint(vector);
    } else if (vector[0] == "f") {
        this->_addIndices(vector);
    } else if (vector[0] == "vn") {
        this->_addNormal(vector);
    };
}

Mesh::Mesh(std::string filename)
{
    std::string         line;
    std::ifstream       input(filename.c_str());

    this->_vertices = new std::vector<GLfloat>();
    this->_normal = new std::vector<GLfloat>();
    this->_indicesVert = new std::vector<GLuint>();
    std::cout << "indice: " << this->_indicesVert << std::endl;
    std::cout << "vertice: " << this->_vertices<< std::endl;
    this->_vertices->clear();
    this->_indicesVert->clear();
    while (std::getline(input, line)) {
    //    std::cout << line << std::endl;
        this->_processLine(line);
    }
    std::cout << this->_vertices->size() << " <- nb vertice" << std::endl;
    std::cout << this->_indicesVert->size() << " <- nb indice" << std::endl;
}
