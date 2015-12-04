#ifndef IMPORTOBJ_HPP
# define IMPORTOBJ_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <map>
# include "vector.hpp"

class					ImportObj
{
	public:
						ImportObj();
						ImportObj( ImportObj const & rhs );
						ImportObj( std::string const filename );
		virtual			~ImportObj();
		
		void			exportMeshData(std::vector<t_vec> & vertices, std::vector<t_vec> & textures, std::vector<t_vec> & normals, std::vector<GLuint> & indices);		

	private:

		void				printV(std::vector< t_vec > v);
		void				printV(std::vector< GLuint > v);
		void				printT(double t);
		int					isDigit(char c);
		GLuint				retrieveNb(std::string str, int *i);
		void				retrieveInd(std::string face, GLuint &v_i, GLuint &vt_i, GLuint &vn_i);
		void				getFace(std::string face);
		void				loadOBJ(const std::string fileName);
		void				rearrange();

	private:
		std::vector< t_vec >				_vertex;
		std::vector< t_vec >				_vertexN;
		std::vector< t_vec2 >				_vertexT;
		std::vector< t_vec >				_vertexOO;
		std::vector< t_vec >				_vertexNOO;
		std::vector< t_vec >				_vertexTOO;
		std::vector< GLuint >				_indices;
		std::vector< GLuint >				_indicesOO;
		std::map< std::string, GLuint >		_triplets;
		GLuint								_curInd;

		std::map<GLuint, t_vec>				_oneNorm;
		std::vector<GLuint>					_vecIds;

};


#endif
