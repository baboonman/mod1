#ifndef MESHGENERATOR_HPP
# define MESHGENERATOR_HPP

# include <map>
# include <vector>

# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "vector.hpp"
# include "HeightmapGenerator.hpp"
# include "Mesh.hpp"

# define THRESHOLD				(100 / 4.0f)
# define GRID					std::vector< std::vector<float> >

class							MeshGenerator		:		public Mesh
{
	public:
								MeshGenerator();
		virtual					~MeshGenerator();

		void					bindVBO(GLuint program);
		void					drawMesh( void ) const;
		void					updateMesh(float t);

	private:
		void					loadMeshGeometry();
};


#endif
