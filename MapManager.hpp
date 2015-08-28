#ifndef MAPMANAGER_HPP
# define MAPMANAGER_HPP

# include "Top.hpp"
//# include "MapMesh.hpp"
# include <cmath>
# include <vector>
# include <fstream>
# include <sstream>

# define GLfloat	float
class MapManager {
    public:
        MapManager(std::string filename, unsigned int sizeX, unsigned int sizeY, float step);
        ~MapManager();
        std::vector<GLfloat>      generateMountain();

    private:
		unsigned int			_sizeX;
		unsigned int			_sizeY;
		float					_step;
		std::vector<Top *>		_lstTop;

        void	    _parseMap(std::string filename);
        void	    _parsePoint( std::string const &line );
 //       GLfloat     _calcHeightPoint(double x, double y);
        GLfloat     _calcHeightPoint(unsigned int x, unsigned int y);
};

#endif
