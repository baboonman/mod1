#ifndef MESH_HPP
# define MESH_HPP

# include <fstream>
# include <sstream>
# include <map>

# define GLFW_INCLUDE_GLCOREARB
# include <GLFW/glfw3.h>

# include "MapManager.hpp"
# include "OpenGlMatrix.hpp"

typedef struct				s_vec
{
	GLfloat					x;
	GLfloat					y;
	GLfloat					z;
}							t_vec;

typedef struct				s_vec2
{
	GLfloat					u;
	GLfloat					v;
}							t_vec2;

class						Mesh
{
	public:
							Mesh();
							Mesh(const GLfloat *mesh);
							Mesh(const std::string fileName);
							Mesh(const GLfloat *mesh, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat *pos);
							Mesh(const std::string fileName, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat *pos);
							~Mesh();
		void				initMesh(GLuint program) const;
		void				initMeshIndices(GLuint program);
		void				initMeshIndicesMAP(GLuint program) const;
		OpenGlMatrix		getModelMatrix( void ) const;
		void				setModelMatrix( OpenGlMatrix newMatrix );
		void				drawMesh( void ) const;
void						sendPosition(unsigned int size, std::vector<t_vec> postion);
        GLuint              *getVBO( void );

	private:
		void				getFace(std::string face);
		void				loadOBJ(const std::string fileName);
		void				rearrange();

	private:
		static int			_i;
		int					_id;
		const GLfloat		*_mesh;
		OpenGlMatrix		_modelMatrix;
		std::vector< t_vec >	_vertex;
		std::vector< t_vec >	_vertexN;
		std::vector< t_vec2 >	_vertexT;
		std::vector< t_vec >	_vertexOO;
		std::vector< t_vec >	_vertexNOO;
		std::vector< t_vec >	_vertexTOO;
		std::vector< GLuint >	_indices;
		std::vector< GLuint >	_indicesOO;
	std::map< std::string, GLuint >		_triplets;
		GLuint				_curInd;

		std::map<GLuint, t_vec>		_oneNorm;
		std::vector<GLuint>			_vecIds;
	
		GLuint					_vao;
		GLuint					_vbo[4];

	protected:
        std::vector<GLfloat>    *_vertices;
        std::vector<GLuint>     *_indicesVert;
        std::vector<GLfloat>    *_normal;
};


#endif
