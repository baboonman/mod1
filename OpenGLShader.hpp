#ifndef OPENGLSHADER_HPP
# define OPENGLSHADER_HPP

# define GLFW_INCLUDE_GLCOREARB
# include <GLFW/glfw3.h>

# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>

class					OpenGLShader
{
	
	public:
		int						addShader( GLenum type, std::string filename );
		int						createProgram( void );
		GLuint					getProgram( void );

		char					*filetobuf( const char *file );
		void					deleteShader( void );
	private:
    	GLuint					_shaderProgram;
		std::vector<GLuint>		_shaders;
};

#endif
