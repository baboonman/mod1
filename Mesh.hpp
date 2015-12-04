#ifndef MESH_HPP
# define MESH_HPP

# include <vector>

# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "vector.hpp"

class								Mesh
{
	public:
									Mesh();
		virtual						~Mesh();

		virtual void				bindVBO(GLuint program) = 0;
		virtual void				drawMesh(void) const = 0;
		virtual void				updateMesh(float t) = 0;

		virtual GLuint *			getVBO(void);
		virtual void				setModelMatrix(OpenGLMatrix const & newMatrix);
		virtual OpenGLMatrix *		getModelMatrix(void);
		virtual std::vector<t_vec>	getVertices();
		virtual std::vector<t_vec>	getNormals();
		virtual std::vector<GLuint>	getIndices();

	protected:
		OpenGLMatrix				_modelMatrix;
		std::vector< t_vec >		_vertices;
		std::vector< t_vec >		_normals;
		std::vector< t_vec >		_textures;
		std::vector< GLuint >		_indices;

		GLuint						_vao;
		GLuint						_vbo[4];
};


#endif
