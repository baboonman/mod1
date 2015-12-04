#ifndef MESHINSTANCE_HPP
# define MESHINSTANCE_HPP

# include <map>
# include <vector>

# include "OpenGLInc.hpp"
# include "Mesh.hpp"

class							MeshInstance		:		public Mesh
{
	public:
								MeshInstance();
								MeshInstance(Mesh & rhs, int nbInstances);
		virtual					~MeshInstance();

		void					bindVBO(GLuint program);
		void					drawMesh( void ) const;
		void					updateMesh(float t);

	private:
		void					loadMeshData(Mesh & mesh);
		int						_nbInstances;	
};

#endif
