#ifndef MESHOBJ_HPP
# define MESHOBJ_HPP

# include <map>
# include <vector>

# include "Mesh.hpp"
# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "vector.hpp"
# include "ImportObj.hpp"

class							MeshObj		:	public Mesh
{
	public:
								MeshObj();
								MeshObj(const std::string fileName);
		virtual					~MeshObj();

		void					bindVBO(GLuint program);
		void					drawMesh( void ) const;
		void					updateMesh(float t);

	private:
		void					loadOBJ(const std::string fileName);

};

#endif
