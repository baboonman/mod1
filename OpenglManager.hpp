#ifndef OPENGLMANAGER_HPP
# define OPENGLMANAGER_HPP

/*      OpenGl includes             */

# define GLFW_INCLUDE_GLCOREARB
# include <GLFW/glfw3.h>

# include <math.h>

# include <iostream>
# include <vector>
# include <string>

# include "OpenCL.hpp"
# include "OpenGlMatrix.hpp"
# include "OpenGLShader.hpp"
# include "Mesh.hpp"

#define FROMFPS(X)		( 1 / X )
#define TOFPS(X)		( 1 / X )

#define AN_INT			FROMFPS(3)

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
		int						initShader(std::string VSFile, std::string FSFile);
		GLuint					getShaderProgram( void );
		int						shouldClose( void );
		void					swap( void );
		void					setUserPtr( t_user_ptr s );
		void					createProjectionMatrix( void );
		OpenGlMatrix			getModelViewMatrix( void );
		void					addMatricesToProgram( OpenGlMatrix model, OpenGlMatrix view, float an, float bn );
		void					run(Mesh mesh);

	private:
		void					initOpenGl( void );

	private:
    	GLFWwindow				*_window;
		OpenGLShader			_shader;
		OpenGlMatrix			_modelviewMatrix;
		OpenGlMatrix			_projectionMatrix;
		t_window_info			_winInfo;
		t_clipping_info			_clipInfo;

};


#endif //OPENGLMANAGER_HPP
