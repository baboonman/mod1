#ifndef HEIGHTMAPGENERATOR_HPP
# define HEIGHTMAPGENERATOR_HPP

# include "vector.hpp"
# include "ProcMap.hpp"

# define THRESHOLD				(100 / 4.0f)
# define GRID					std::vector< std::vector<float> >

class							HeightmapGenerator
{
	public:
								HeightmapGenerator();
		virtual					~HeightmapGenerator();

		void					exportMeshData(std::vector<t_vec> & vertices, std::vector<t_vec> & normals, std::vector<GLuint> & indices);

	private:
		void					createTerrain(bool top);
//		void					createMap(GRID &map, int gridSize, float height, std::vector<t_vec> tops, float t);
		void					computeVert(GRID &map, int gridSize);
		void					computeNorm();
		void					computeIndices();
		float					calcUp(int i, int j, t_vec top);
		t_vec					getNorm(GLuint center, GLuint first, GLuint second);

		int						_gridSize;
		float					_maxHeight;
		std::vector< t_vec >	_vertex;
		std::vector< t_vec >	_vertexN;
		std::vector< GLuint >	_indices;

};

#endif
