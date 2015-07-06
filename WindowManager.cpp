#include "WindowManager.hpp"

WindowManager::WindowManager( int sizeX, int sizeY) 
 : projectionMatrix(4, 4), modelViewMatrix(4, 4), rotationMatrix(4, 4), finalProjMatrix(4, 4), _sizeX(sizeX), _sizeY(sizeY) {

    std::string     *vertexShader;
    std::string     *fragmentShader;

    this->zFar = 100.0f;
    this->zNear = 1.0f;
    this->aspectRatio = 1.0f;
    this->viewAngle = 0.25f * M_PI;
    this->meshManager = new MeshManager(20, 20);

    this->_rotX = 0;
    this->_rotY = 0;
    this->_rotZ = 0;

    glfwSetErrorCallback(WindowManager::errorCallback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 4);
  //  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
  //  glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->window = glfwCreateWindow(sizeX, sizeY, "MOD1", NULL, NULL);

    if (!window)
        exit(EXIT_FAILURE);

    glfwSetKeyCallback(this->window, keyCallback);
    glfwMakeContextCurrent(this->window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    vertexShader = this->getShader("shaders/vertext.ver");
    fragmentShader = this->getShader("shaders/fragment.frag");
    this->shaderProgram = this->makeShaderProgram(vertexShader, fragmentShader);

    if (this->shaderProgram == 0u)
        exit(EXIT_FAILURE);

    glUseProgram(this->shaderProgram);
    this->ulocProject   = glGetUniformLocation(this->shaderProgram, "proj");
    this->ulocRot = glGetUniformLocation(this->shaderProgram, "rotation");

    this->initMatrix();
    this->finalProjMatrix(0, 0) = 1.79259;
    this->finalProjMatrix(0, 1) = 0;
    this->finalProjMatrix(0, 2) = 0;
    this->finalProjMatrix(0, 3) = 0;
    this->finalProjMatrix(1, 0) = 0;
    this->finalProjMatrix(1, 1) = 1.79259;
    this->finalProjMatrix(1, 2) = 0;
    this->finalProjMatrix(1, 3) = 0;
    this->finalProjMatrix(2, 0) = 0;
    this->finalProjMatrix(2, 1) = 0;
    this->finalProjMatrix(2, 2) = -1.00200;
    this->finalProjMatrix(2, 3) = 4.80981;
    this->finalProjMatrix(3, 0) = 0;
    this->finalProjMatrix(3, 1) = 0;
    this->finalProjMatrix(3, 2) = -1.00000;
    this->finalProjMatrix(3, 3) = 5.00000;

    std::cout << this->finalProjMatrix << std::endl;
    glUniformMatrix4fv(this->ulocProject, 1, GL_FALSE, this->finalProjMatrix.toGLfloat());
    glUniformMatrix4fv(this->ulocRot, 1, GL_FALSE, this->rotationMatrix.toGLfloat());

 //   this->meshManager->makeMesh(this->shaderProgram);

    this->run();

}

void        WindowManager::run() {
    this->frame = 0;
    this->iter = 0;
    this->lastUpdateTime = glfwGetTime();
    GLuint  nbTriangle;

    glViewport(0 ,0 ,this->_sizeX * 2, this->_sizeY * 2);
    nbTriangle = this->meshManager->getNbIndices();
    this->meshManager->makeMesh(this->shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        ++this->frame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //       this->meshManager->makeMesh(this->shaderProgram);
        //glViewport(0 ,0 ,this->_sizeX * 4, this->_sizeY * 4);
        glDrawElements(GL_TRIANGLES, nbTriangle, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        this->dt = glfwGetTime();
        if ((this->dt - this->lastUpdateTime) > 0.01f)
        {
            this->_rotX += 0.01;
            this->_rotY += 0.02;
            this->_rotZ += 0.04;
            GenerateMatrix::setRotation(this->rotationMatrix, this->_rotX, this->_rotY, this->_rotZ);
            glUniformMatrix4fv(this->ulocRot, 1, GL_FALSE, this->rotationMatrix.toGLfloat());
            this->iter++;

            this->lastUpdateTime = dt;
            this->frame = 0;
        }
    }
}

void        WindowManager::initMatrix() {
    GenerateMatrix::setProjection(
        this->projectionMatrix,
        this->viewAngle,
        this->aspectRatio,
        this->zFar,
        this->zNear
    );

    GenerateMatrix::setModelView(this->modelViewMatrix);
    GenerateMatrix::setRotation(this->rotationMatrix, this->_rotX, this->_rotY, this->_rotZ);
    this->finalProjMatrix = this->projectionMatrix * this->modelViewMatrix;
    std::cout << "Projection matrix: " << std::endl;
    std::cout << this->projectionMatrix << std::endl;
    std::cout << "ModelView matrix: " << std::endl;
    std::cout << this->modelViewMatrix << std::endl;
}

void    WindowManager::errorCallback(int error, const char* description)
{
    std::cerr << "Error(" << error << "): " << description << std::endl;
}

GLuint      WindowManager::makeShaderProgram(const std::string* vsText, const std::string* fsText)
{
    GLuint program = 0u;
    GLint programOk;
    GLuint vertexShader = 0u;
    GLuint fragmentShader = 0u;
    GLsizei logLength;
    char infoLog[8192];

    vertexShader = this->makeShader(GL_VERTEX_SHADER, vsText);
    if (vertexShader != 0u)
    {
        fragmentShader = this->makeShader(GL_FRAGMENT_SHADER, fsText);
        if (fragmentShader != 0u)
        {
            /* make the program that connect the two shader and link it */
            program = glCreateProgram();
            if (program != 0u)
            {
                /* attach both shader and link */
                glAttachShader(program, vertexShader);
                glAttachShader(program, fragmentShader);
                glLinkProgram(program);
                glGetProgramiv(program, GL_LINK_STATUS, &programOk);

                if (programOk != GL_TRUE)
                {
                    std::cerr << "ERROR, failed to link shader program" << std::endl;
                    glGetProgramInfoLog(program, 8192, &logLength, infoLog);
                    std::cerr << "ERROR:" << std::endl;
                    std::cerr << infoLog << std::endl;
                    glDeleteProgram(program);
                    glDeleteShader(fragmentShader);
                    glDeleteShader(vertexShader);
                    program = 0u;
                }
            }
        }
        else
        {
            std::cerr << "ERROR: Unable to load fragment shader" << std::endl;
            glDeleteShader(vertexShader);
        }
    }
    else
    {
        std::cerr << "ERROR: Unable to load vertex shader" << std::endl;
    }
    return program;
}

GLuint      WindowManager::makeShader(GLenum type, const std::string* text)
{
    GLuint shader;
    GLint shaderOk;
    GLsizei logLength;
    char infoLog[8192];
    const char  *srcShader;
    
    srcShader = text->c_str();

    shader = glCreateShader(type);
    if (shader != 0)
    {
        glShaderSource(shader, 1, (const GLchar**)(&srcShader), NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderOk);
        if (shaderOk != GL_TRUE)
        {
            std::cerr << "ERROR: Failed to compile " << ((type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex") << " shader" << std::endl;
            glGetShaderInfoLog(shader, 8192, &logLength,infoLog);
            fprintf(stderr, "ERROR: \n%s\n\n", infoLog);
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

void        WindowManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << "Code: " << key << " scan code : " << scancode << " action: " << action << " mods " << mods << std::endl;
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            /* Exit program on Escape */
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
    }
}

std::string     *WindowManager::getShader(std::string filename) const {
    std::ifstream ifs(filename);
    std::string *content;
        
    content = new std::string( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()) );
    return content;
}

WindowManager::~WindowManager( void ) {
    glfwTerminate();
    delete this->meshManager;
}

