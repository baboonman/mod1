#ifndef MESH_HPP
# define MESH_HPP

# include <fstream>
# include <sstream>
# include <map>
# include <vector>

# define GLFW_INCLUDE_GLCOREARB
# include <GLFW/glfw3.h>

# include "OpenGLMatrix.hpp"
# include "vector.hpp"
# include "Noise.hpp"

# define SIDE			100
# define THRESHOLD		(SIDE / 4.0f)
# define HEIGHT			(SIDE / 2.0f)

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
		OpenGLMatrix		getModelMatrix( void ) const;
		void				setModelMatrix( OpenGLMatrix newMatrix );
		void				drawMesh( void ) const;
void						sendPosition(unsigned int size, std::vector<t_vec> postion);
        GLuint              *getVBO( void );
        void                setNbParticles( int n );

		void				updateTerrain(float t);

		std::vector< t_vec >	getPosition() const;
	private:
		void				getFace(std::string face);
		void				loadOBJ(const std::string fileName);
		void				rearrange();

void				computeIndices();
void				computeNorm(float map[SIDE][SIDE]);
void				computeVert(float map[SIDE][SIDE]);
void				createTerrain();
void				createMap(float map[SIDE][SIDE], std::vector<t_vec> tops, float t);
float				calcUp(int i, int j, t_vec top);
t_vec				getNorm(GLuint center, GLuint first, GLuint second);

	private:
		static int			_i;
		int					_id;
		const GLfloat		*_mesh;
		OpenGLMatrix		_modelMatrix;
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

		int                     _nbParticles;

};

std::ostream&				operator<<(std::ostream& flux, Mesh const& m);

#endif
