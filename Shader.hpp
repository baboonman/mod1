#ifndef SHADER_HPP
# define SHADER_HPP

# include <glad/glad.h>
# include <GLFW/glfw3.h>


class					Shader
{
	
	public:
								Shader();
								~Shader();
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
