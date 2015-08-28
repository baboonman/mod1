#include "OpenglManager.hpp"
/*
void	compute_proj_mat(GLuint prog, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLint			uloc_project;
    float f;

    uloc_project   = glGetUniformLocation(prog, "project");

    f = 1.0f / tanf(fovy / 2.0f);
    projection_matrix[0]  = f / aspect;
    projection_matrix[5]  = f;
    projection_matrix[10] = (zFar + zNear)/ (zNear - zFar);
    projection_matrix[11] = -1.0f;
    projection_matrix[14] = 2.0f * (zFar * zNear) / (zNear - zFar);
    glUniformMatrix4fv(uloc_project, 1, GL_FALSE, projection_matrix);
}

void compute_movi_mat(GLuint prog, GLfloat x, GLfloat y, GLfloat z)
{
	GLint			uloc_modelview;

    uloc_modelview = glGetUniformLocation(prog, "modelview");

    modelview_matrix[5]  = cos(alpha);
    modelview_matrix[6]  = sin(alpha);
    modelview_matrix[9]  = -sin(alpha);
    modelview_matrix[10]  = cos(alpha);

    modelview_matrix[12]  = x;
    modelview_matrix[13]  = y;
    modelview_matrix[14]  = z;
    glUniformMatrix4fv(uloc_modelview, 1, GL_FALSE, modelview_matrix);
}
*/
OpenGlMatrix			OpenglManager::getModelViewMatrix(void)
{
	return _modelviewMatrix;
}

void					OpenglManager::createProjectionMatrix(void)
{
	_clipInfo.aspect = _winInfo.width / _winInfo.height;
	_projectionMatrix.computeProjectionMatrix(_clipInfo.fov, _clipInfo.aspect, _clipInfo.zNear, _clipInfo.zFar);
}
/*
void					OpenglManager::addMatricesToProgram(void)
{
	GLint			uloc_project;
	GLint			uloc_modelview;

    uloc_project   = glGetUniformLocation(_shaderProgram, "project");
    uloc_modelview = glGetUniformLocation(_shaderProgram, "modelview");
    glUniformMatrix4fv(uloc_project, 1, GL_FALSE, _projectionMatrix.getMatrixArray());
    glUniformMatrix4fv(uloc_modelview, 1, GL_FALSE, _modelviewMatrix.getMatrixArray());
}

void					OpenglManager::addMatricesToProgram(OpenGlMatrix matrix)
{
	GLint			uloc_project;
	GLint			uloc_modelview;

    uloc_project   = glGetUniformLocation(_shaderProgram, "project");
    uloc_modelview = glGetUniformLocation(_shaderProgram, "modelview");
    glUniformMatrix4fv(uloc_project, 1, GL_FALSE, _projectionMatrix.getMatrixArray());
    glUniformMatrix4fv(uloc_modelview, 1, GL_FALSE, matrix.getMatrixArray());
}

void					OpenglManager::addMatricesToProgram(OpenGlMatrix model, OpenGlMatrix view)
{
//	GLint			uloc_MVP;
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;
//	OpenGlMatrix	mat;

//    uloc_MVP = glGetUniformLocation(_shaderProgram, "MVP");
    uloc_M = glGetUniformLocation(_shaderProgram, "M");
    uloc_V = glGetUniformLocation(_shaderProgram, "V");
    uloc_P = glGetUniformLocation(_shaderProgram, "P");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, _projectionMatrix.getMatrixArray());
//	mat.multMatrix(model.multMatrix(view.multMatrix(_projectionMatrix)));
//    glUniformMatrix4fv(uloc_MVP, 1, GL_FALSE, mat.getMatrixArray());
}
*/
void					OpenglManager::addMatricesToProgram(OpenGlMatrix model, OpenGlMatrix view, float an, float bn)
{
//	GLint			uloc_MVP;
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;
	GLint			uloc_AN;
	GLint			uloc_BN;
//	OpenGlMatrix	mat;

//    uloc_MVP = glGetUniformLocation(_shaderProgram, "MVP");
    uloc_M = glGetUniformLocation(_shaderProgram, "M");
    uloc_V = glGetUniformLocation(_shaderProgram, "V");
    uloc_P = glGetUniformLocation(_shaderProgram, "P");
    uloc_AN = glGetUniformLocation(_shaderProgram, "AN");
    uloc_BN = glGetUniformLocation(_shaderProgram, "BN");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, _projectionMatrix.getMatrixArray());
    glUniform1f(uloc_AN, an);
    glUniform1f(uloc_BN, bn);
//	mat.multMatrix(model.multMatrix(view.multMatrix(_projectionMatrix)));
//    glUniformMatrix4fv(uloc_MVP, 1, GL_FALSE, mat.getMatrixArray());
}

/*
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	t_user_ptr				*ptr;

	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
	ptr->winInfo->width = w;
	ptr->winInfo->height = h;
    glViewport(0, 0, w, h);
}
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_user_ptr				*ptr;
	float					alpha = 0.5235f;

	(void)scancode;
	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
//	std::cout << "Duuh 1/3: " << ptr->test << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	else if (action == GLFW_PRESS)
	{
		if (mods == GLFW_MOD_SHIFT)
		{
			if (key == GLFW_KEY_LEFT)
				ptr->model->rotateZ(alpha);
			else if (key == GLFW_KEY_RIGHT)
				ptr->model->rotateZ(-alpha);
		}
		else
		{
			if (key == GLFW_KEY_UP)
				ptr->model->rotateX(alpha);
			else if (key == GLFW_KEY_DOWN)
				ptr->model->rotateX(-alpha);
			else if (key == GLFW_KEY_LEFT)
				ptr->model->rotateY(alpha);
			else if (key == GLFW_KEY_RIGHT)
				ptr->model->rotateY(-alpha);
		}
	}
/*	if (action == GLFW_PRESS)
	{
		if (mods == GLFW_MOD_CONTROL)
		{
			if (key == GLFW_KEY_UP && mods == GLFW_MOD_SHIFT)
			{
			}
			else if (key == GLFW_KEY_UP)
			{
				alpha -= 0.1 * M_PI;
			}
			if (key == GLFW_KEY_DOWN && mods == GLFW_MOD_SHIFT)
			{
			}
			else if (key == GLFW_KEY_DOWN)
			{
				alpha += 0.1 * M_PI;

			}
			if (key == GLFW_KEY_LEFT)
			{

				break;
			}
			if (key == GLFW_KEY_RIGHT)
			{
			}
		}
		else 
		{
			if (key == GLFW_KEY_UP && mods == GLFW_MOD_SHIFT)
			{
				v_y -= 1;
			}
			else if (key == GLFW_KEY_UP)
			{
				v_z += 1;
			}
			if (key == GLFW_KEY_DOWN && mods == GLFW_MOD_SHIFT)
			{
				v_y += 1;
			}
			else if (key == GLFW_KEY_DOWN)
			{
				v_z -= 1;
			}
			if (key == GLFW_KEY_LEFT)
			{
				v_x += 1;
			}
			if (key == GLFW_KEY_RIGHT)
			{
				v_x -= 1;
			}
		}
	}
	 = *(reinterpret_cast<GLuint *>(glfwGetWindowUserPointer(window)));
	compute_movi_mat(prog, v_x, v_y, v_z);
	*/
}

void error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}

void				OpenglManager::initOpenGl( void )
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(0);
    
	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
//	glfwWindowHint(GLFW_DEPTH_BITS, 24);
  	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_winInfo.width, _winInfo.height, _winInfo.winName.c_str(), NULL, NULL);
    if (! _window )
    {
        glfwTerminate();
        exit(0);
    }
    
    /* Register events callback */
 //   glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetKeyCallback(_window, key_callback);
//    glfwSetCursorPosCallback(_window, cursor_position_callback);
//    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
  //  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
    glViewport(0, 0, _winInfo.width, _winInfo.height);
}

OpenglManager::OpenglManager()
{
	_winInfo.width = 512;
	_winInfo.height = 512;
	_winInfo.winName = "Untitled";
	_clipInfo.fov = 45;
	_clipInfo.aspect = 1;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 100;
	initOpenGl();
}

OpenglManager::OpenglManager( GLfloat width, GLfloat height, std::string winName ) 
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = 45;
	_clipInfo.aspect = 1;
	_clipInfo.zNear = 1;
	_clipInfo.zFar = 100;
	initOpenGl();
}

OpenglManager::OpenglManager( GLfloat width, GLfloat height, std::string winName,
				 			   GLfloat fov, GLfloat aspect, 
							   GLfloat zNear, GLfloat zFar )
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = fov;
	_clipInfo.aspect = aspect;
	_clipInfo.zNear = zNear;
	_clipInfo.zFar = zFar;
	initOpenGl();
}

OpenglManager::~OpenglManager()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

char		*OpenglManager::filetobuf(const char *file)
{
    FILE *fptr;
    long length;
    char *buf;
    
    fptr = fopen(file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length+1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
   
    return buf;
}

void			OpenglManager::deleteShader()
{
	for (auto i = _shaders.begin() ; i < _shaders.end() ; i++)
	{
		glDeleteShader(*i);
	}
}

int				OpenglManager::addShader(GLenum type, std::string filename)
{
    GLuint shader;
    GLint shader_ok;
    GLsizei log_length;
    char info_log[8192];
	const char	*source;
    
	source = filetobuf(filename.c_str());
    shader = glCreateShader(type);
    if (shader != 0)
    {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
        if (shader_ok != GL_TRUE)
        {
            fprintf(stderr, "ERROR: Failed to compile %s shader\n", (type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex" );
            glGetShaderInfoLog(shader, 8192, &log_length,info_log);
            fprintf(stderr, "ERROR: \n%s\n\n", info_log);
            glDeleteShader(shader);
            shader = 0;
			return (0);
        }
    }
	_shaders.push_back(shader);
    return (1);
}

int				OpenglManager::createProgram()
{
    GLuint program = 0u;
    GLint program_ok;
    GLsizei log_length;
    char info_log[8192];
    
    /* make the program that connect the two shader and link it */
    program = glCreateProgram();
    if (program != 0u)
    {
		/* attach both shader and link */
		for (auto i = _shaders.begin() ; i < _shaders.end() ; i++)
		{
			glAttachShader(program, *i);
		}
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
		if (program_ok != GL_TRUE)
		{
			fprintf(stderr, "ERROR, failed to link shader program\n");
			glGetProgramInfoLog(program, 8192, &log_length, info_log);
			fprintf(stderr, "ERROR: \n%s\n\n", info_log);
			glDeleteProgram(program);
			deleteShader();
			program = 0u;
			return (0);
		}
    }
	_shaderProgram = program;
    return (1);
}

GLuint			OpenglManager::getProgram()
{
	return (_shaderProgram);
}

int				OpenglManager::shouldClose()
{
	return (glfwWindowShouldClose(_window));
}

void				OpenglManager::swap()
{
        glfwSwapBuffers(_window);
}

void				OpenglManager::setUserPtr(t_user_ptr s)
{
	glfwSetWindowUserPointer(_window, static_cast<void *>(&s));
}
