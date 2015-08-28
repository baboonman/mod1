#include "MapManager.hpp"

MapManager::MapManager(std::string filename, unsigned int sizeX, unsigned int sizeY, float step) 
: _sizeX(sizeX), _sizeY(sizeY), _step(step) {
    this->_parseMap(filename);
}

MapManager::~MapManager() {
}

void	MapManager::_parseMap( std::string filename)
{
	std::string		line;
	std::ifstream	mapFile(filename);

	if (mapFile.is_open())
	{
		while (std::getline(mapFile,line))
		{
			this->_parsePoint(line);
		}
		mapFile.close();
	}
	else
		std::cerr << "Unable to open the file: " << filename << std::endl;
}

void	MapManager::_parsePoint( std::string const &line )
{
	std::stringstream	ss(line);
	std::string			point;
	int					i;
	unsigned int	    x;
	unsigned int	    y;

	i = 0;
	while (getline(ss, point, ' '))
	{
		switch (i) {
			case 0:
				x = std::stoi(point);
				if (x >= this->_sizeX)
					throw new std::exception();
				break;
			case 1:
				y = std::stoi(point);
				if (y >= this->_sizeY)
				{
					std::cout << "y: " << y << std::endl;
					throw new std::exception();
				}
				break;
			case 2:
				this->_lstTop.push_back(new Top(std::stod(point), x, y));
				break;
		}
		if (++i > 2)
			i = 0;
	}
}

std::vector<GLfloat> MapManager::generateMountain() {
	std::vector<GLfloat>	map;
    for (unsigned int x = 0; x < this->_sizeX; x++) {
        for (unsigned int y = 0; y < this->_sizeY; y++) {
            map.push_back(this->_calcHeightPoint(x, y));
//			map[x][y] = this->_calcHeightPoint(x, y);
        }
    }
	return map;
}

GLfloat     MapManager::_calcHeightPoint(unsigned int x, unsigned int y)
{
	GLfloat res;
	GLfloat tmp;
	Top		*top;
	GLfloat xf;
	GLfloat yf;
	GLfloat sizeXf;
	GLfloat sizeYf;

    xf = static_cast<GLfloat>(x);
    yf = static_cast<GLfloat>(y);
    sizeXf = static_cast<GLfloat>(this->_sizeX);
    sizeYf = static_cast<GLfloat>(this->_sizeY);
	res = 0;
	for (auto it = this->_lstTop.begin(); it < this->_lstTop.end(); it++)
	{
		top = *it;
		tmp = top->high * exp(-(top->xCenter - x) * (top->xCenter - x) / sizeXf
				- (top->yCenter - y) * (top->yCenter - y) / sizeYf);

		res += tmp;

	}

	return (res);
}
