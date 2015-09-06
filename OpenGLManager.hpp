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
# include "OpenGLMatrix.hpp"
# include "OpenGLShader.hpp"
# include "Mesh.hpp"
# include "CameraControl.hpp"

#define FROMFPS(X)		( 1.0f / X )
#define TOFPS(X)		( 1.0f / X )

#define AN_INT			FROMFPS(3.0f)

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
	t_window_info				*winInfo;
	OpenGLMatrix				*model;
	CameraControl				*camera;
}								t_user_ptr;

class							OpenGLManager
{
	public:
								OpenGLManager();
								OpenGLManager( GLfloat width, GLfloat height, std::string winName );
								OpenGLManager( GLfloat width, GLfloat height, std::string winName,
											   GLfloat fov, GLfloat aspect, 
											   GLfloat zNear, GLfloat zFar );
								~OpenGLManager();
//		int						initShader(std::string VSFile, std::string GSFile, std::string FSFile);
		int						initShader(std::string VSFile, std::string FSFile);
		GLuint					getShaderProgram( void );
		int						shouldClose( void );
		void					swap( void );
		void					setUserPtr( t_user_ptr *s );
		void					createProjectionMatrix( void );
		void					addMatricesToProgram( OpenGLMatrix model, OpenGLMatrix view, float an, float bn );
		void					run(CameraControl *cam, Mesh mesh);
        void                    setNbParticles( int n );

	private:
		void					initOpenGl( void );

	private:
    	GLFWwindow				*_window;
		OpenGLShader			_shader;
		OpenGLMatrix			_projectionMatrix;
		t_window_info			_winInfo;
		t_clipping_info			_clipInfo;
		int                     _nbParticles;

};


#endif //OPENGLMANAGER_HPP
