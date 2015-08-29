#include "OpenglManager.hpp"

OpenGlMatrix			OpenglManager::getModelViewMatrix(void)
{
	return _modelviewMatrix;
}

void					OpenglManager::createProjectionMatrix(void)
{
	_clipInfo.aspect = _winInfo.width / _winInfo.height;
	_projectionMatrix.computeProjectionMatrix(_clipInfo.fov, _clipInfo.aspect, _clipInfo.zNear, _clipInfo.zFar);
}

void					OpenglManager::addMatricesToProgram(OpenGlMatrix model, OpenGlMatrix view, float an, float bn)
{
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;
	GLint			uloc_AN;
	GLint			uloc_BN;

    uloc_M = glGetUniformLocation(this->_shader.getProgram(), "M");
    uloc_V = glGetUniformLocation(this->_shader.getProgram(), "V");
    uloc_P = glGetUniformLocation(this->_shader.getProgram(), "P");
    uloc_AN = glGetUniformLocation(this->_shader.getProgram(), "AN");
    uloc_BN = glGetUniformLocation(this->_shader.getProgram(), "BN");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, _projectionMatrix.getMatrixArray());
    glUniform1f(uloc_AN, an);
    glUniform1f(uloc_BN, bn);
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

int					OpenglManager::initShader(std::string VSFile, std::string FSFile)
{
	this->_shader.addShader(GL_VERTEX_SHADER, VSFile);
	this->_shader.addShader(GL_FRAGMENT_SHADER , FSFile);
	if (! this->_shader.createProgram())
		return (0);
    glUseProgram(this->_shader.getProgram());
	return (1);
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

GLuint				OpenglManager::getShaderProgram( void ) { return (this->_shader.getProgram()); }

void				OpenglManager::run(Mesh mesh)
{
	float			an = 0.0f, bn = 0.0f;
	double			before;
	double			after;
	double			an_mod;
	double			fps = 0.0;
	OpenCL			openCL(100, 2.0f, 40, 10, 10, 10);
	OpenGlMatrix	modelMat;
	t_user_ptr		userPtr;

	openCL.initOpenCL(mesh.getVBO()[3]);

	this->_modelviewMatrix.translate(0.0, 0.0, -5.0);
	this->createProjectionMatrix();

	modelMat = mesh.getModelMatrix();
	userPtr.model = &modelMat;
	userPtr.test = "IEEEE";
	this->setUserPtr(userPtr);

    while (!this->shouldClose())
    {
		before = glfwGetTime();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->addMatricesToProgram(modelMat, this->_modelviewMatrix, an, bn);

		mesh.drawMesh();
		this->swap();

		glfwPollEvents();

		glFinish();
        openCL.executeKernel();

		if (before - an_mod > AN_INT) {
			an += 1;
			an_mod = glfwGetTime();
		}
		bn += 0.01f;

		after = glfwGetTime();
		before = 1 / ((after - before)) ;
		if ((before - fps) > 1.0 || (before - fps) < -1.0) {
			fps = before;
		//	std::cerr << fps << std::endl;
		}
	}
}
