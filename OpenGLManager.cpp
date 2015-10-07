#include "OpenGLManager.hpp"

void					OpenGLManager::createProjectionMatrix(void)
{
	this->_clipInfo.aspect = this->_winInfo.width / this->_winInfo.height;
	this->_projectionMatrix.computeProjectionMatrix(this->_clipInfo.fov, this->_clipInfo.aspect, this->_clipInfo.zNear, this->_clipInfo.zFar);
}

void					OpenGLManager::addMatricesToProgram(OpenGLMatrix model, OpenGLMatrix view, float an, float bn)
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
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, this->_projectionMatrix.getMatrixArray());
    glUniform1f(uloc_AN, an);
    glUniform1f(uloc_BN, bn);
}

				
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	t_user_ptr				*ptr;

	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
	ptr->winInfo->width = w;
	ptr->winInfo->height = h;
    glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_user_ptr				*ptr;
	float					alpha = 0.5235f;

	(void)scancode;
	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	ptr->camera->controlKey(key, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	t_user_ptr				*ptr;
	
	ptr = reinterpret_cast<t_user_ptr *>(glfwGetWindowUserPointer(window));
	ptr->camera->controlMouse(xPos, yPos);
}

void error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}

void				OpenGLManager::initOpenGl( void )
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(0);
    
	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
//	glfwWindowHint(GLFW_DEPTH_BITS, 24);
  	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->_window = glfwCreateWindow(this->_winInfo.width, this->_winInfo.height, this->_winInfo.winName.c_str(), NULL, NULL);
    if (! this->_window )
    {
        glfwTerminate();
        exit(0);
    }
    
    glfwSetFramebufferSizeCallback(this->_window, framebuffer_size_callback);
    glfwSetKeyCallback(this->_window, key_callback);
    glfwSetCursorPosCallback(this->_window, cursor_position_callback);
	glfwSetInputMode(this->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    glfwMakeContextCurrent(this->_window);
    glfwSwapInterval(1);
  //  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glViewport(0, 0, this->_winInfo.width, this->_winInfo.height);
}

//int					OpenGLManager::initShader(std::string VSFile, std::string GSFile, std::string FSFile)
int					OpenGLManager::initShader(std::string VSFile, std::string FSFile)
{
	this->_shader.addShader(GL_VERTEX_SHADER, VSFile);
//	this->_shader.addShader(GL_GEOMETRY_SHADER, GSFile);
	this->_shader.addShader(GL_FRAGMENT_SHADER , FSFile);
	if (! this->_shader.createProgram())
		return (0);
    glUseProgram(this->_shader.getProgram());
	return (1);
}

OpenGLManager::OpenGLManager()
{
	_winInfo.width = 512;
	_winInfo.height = 512;
	_winInfo.winName = "Untitled";
	_clipInfo.fov = 45;
	_clipInfo.aspect = 1;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 10000;
	initOpenGl();
}

OpenGLManager::OpenGLManager( GLfloat width, GLfloat height, std::string winName ) 
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = 45;
	_clipInfo.aspect = width / height;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 1000;
	initOpenGl();
}

OpenGLManager::OpenGLManager( GLfloat width, GLfloat height, std::string winName,
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

OpenGLManager::~OpenGLManager()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int				OpenGLManager::shouldClose()
{
	return (glfwWindowShouldClose(_window));
}

void				OpenGLManager::swap()
{
	glfwSwapBuffers(_window);
}

void				OpenGLManager::setUserPtr(t_user_ptr *s)
{
	glfwSetWindowUserPointer(_window, static_cast<void *>(s));
}

GLuint				OpenGLManager::getShaderProgram( void ) { return (this->_shader.getProgram()); }

void				OpenGLManager::run(CameraControl *cam, Mesh *mesh)
{
	float			an = 50.0f, bn = 0.0f, t = 0.f;
	double			before;
	double			after;
	double			an_mod;
	double			fps = 0.0;
	OpenCL			openCL(this->_nbParticles, 2.0f, 4000, 30, 30, 30);
	OpenGLMatrix	*modelMat;
	t_user_ptr		*userPtr = new t_user_ptr;


    openCL.initOpenCL(mesh->getVBO()[3]);

//	this->_modelviewMatrix.translate(0.0, 0.0, -5.0);
	this->createProjectionMatrix();

	userPtr->winInfo = &this->_winInfo;
	modelMat = mesh->getModelMatrix();
	userPtr->model = modelMat;
	userPtr->camera = cam;
	this->setUserPtr(userPtr);

    while (!this->shouldClose())
    {
		before = glfwGetTime();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->addMatricesToProgram(*modelMat, cam->getViewMatrix(), an, bn);


		mesh->updateMesh(t);
		mesh->drawMeshInstanced();
		this->swap();

		glfwPollEvents();


		after = glfwGetTime();

		t += .2f;
		if (t > 1024.f)
			t = 0.f;

		glFinish();
        openCL.executeKernel();
	}
	openCL.release();
}

void				OpenGLManager::run(CameraControl *cam, OpenGLScene *scene)
{
	float			t = 0.f;
	double			fps = 0.0;
	OpenGLMatrix	modelMat;
	t_user_ptr		*userPtr = new t_user_ptr;

	this->createProjectionMatrix();

	userPtr->winInfo = &this->_winInfo;
	userPtr->model = scene->getModelMatrix();
	userPtr->camera = cam;
	this->setUserPtr(userPtr);

    while (!this->shouldClose())
    {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->drawScene(cam->getViewMatrix(), this->_projectionMatrix, t);

		t += .2f;
		if (t > 1024.f)
			t = 0.f;

		this->swap();
		glfwPollEvents();
	}
}

void                    OpenGLManager::setNbParticles( int n ) {
    this->_nbParticles = n;
}
