#ifndef MAP_MANAGER_HPP
# define MAP_MANAGER_HPP

# include "MapMesh.hpp"
# include "Top.hpp"
# include <cmath>

class MapManager {
    public:
        MapManager(std::string filename, MapMesh *mapMesh);
        ~MapManager();
        void        generateMountain();
    private:
        MapMesh                 *_map;
		std::vector<Top *>		_lstTop;

        void	    _parseMap(std::string filename);
        void	    _parsePoint( std::string const &line );
        GLfloat     _calcHeightPoint(double x, double y);
        GLfloat     _calcHeightPoint(unsigned int x, unsigned int y);
};

#endif
