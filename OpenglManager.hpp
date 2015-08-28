#ifndef OPENGLMANAGER_HPP
# define OPENGLMANAGER_HPP

/*      OpenGl includes             */

//# define GLFW_INCLUDE_NONE
# define GLFW_INCLUDE_GLCOREARB

//# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include <iostream>
# include <vector>
# include <string>

# include "OpenGlMatrix.hpp"

//void glVertexAttribDivisor(GLuint index,  GLuint divisor);

//void	compute_proj_mat(GLuint prog, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
//void 	compute_movi_mat(GLuint prog, GLfloat x, GLfloat y, GLfloat z);
/*
	static GLfloat			alpha = 0.0f;

	static GLfloat			v_x = -5.0f;
	static GLfloat			v_y = -5.0f;
	static GLfloat			v_z = -20.0f;

	static GLfloat			w_width = 512.0f;
	static GLfloat			w_height = 512.0f;

	static	GLfloat			fov = 45.0f;
	static	GLfloat			aspect_ratio = w_width / w_height;
	static	GLfloat			z_near = 1.0f;
	static	GLfloat			z_far = 100.f;
*/
/*
	static	GLfloat			modelview_matrix[16] = {
	    1.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 1.0f
	};

	static	GLfloat			projection_matrix[16] = {
	    1.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 1.0f
	};
*/

typedef struct					s_window_info
{
	GLfloat						width;
	GLfloat						height;
	std::string					winName;
}								t_window_info;

typedef struct					s_clipping_info
{
	GLfloat						fov;
	GLfloat						aspect;
	GLfloat						zNear;
	GLfloat						zFar;
}								t_clipping_info;

typedef struct					s_user_ptr
{
//	t_window_info				*winInfo;
	OpenGlMatrix				*model;
	std::string					test;
}								t_user_ptr;

class							OpenglManager
{
	public:
								OpenglManager();
								OpenglManager( GLfloat width, GLfloat height, std::string winName );
								OpenglManager( GLfloat width, GLfloat height, std::string winName,
											   GLfloat fov, GLfloat aspect, 
											   GLfloat zNear, GLfloat zFar );
								~OpenglManager();
		int						addShader( GLenum type, std::string filename );
		int						createProgram( void );
		GLuint					getProgram( void );
		int						shouldClose( void );
		void					swap( void );
		void					setUserPtr( t_user_ptr s );
		void					createProjectionMatrix( void );
		OpenGlMatrix			getModelViewMatrix( void );
		void					addMatricesToProgram( void );
		void					addMatricesToProgram( OpenGlMatrix matrix );
		void					addMatricesToProgram( OpenGlMatrix model, OpenGlMatrix view );
		void					addMatricesToProgram( OpenGlMatrix model, OpenGlMatrix view, float an, float bn );

	private:
		void					initOpenGl( void );
		char					*filetobuf( const char *file );
		void					deleteShader( void );

	private:
    	GLFWwindow				*_window;
    	GLuint					_shaderProgram;
		std::vector<GLuint>		_shaders;
		OpenGlMatrix			_modelviewMatrix;
		OpenGlMatrix			_projectionMatrix;
		t_window_info			_winInfo;
		t_clipping_info			_clipInfo;

};


#endif //OPENGLMANAGER_HPP
