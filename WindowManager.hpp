#ifndef WINDOWMANAGER_HPP
# define WINDOWMANAGER_HPP

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cstdlib>
# include <iostream>
# include <string>
# include <unistd.h>
# include "GenerateMatrix.hpp"
# include "MeshManager.hpp"
#include <fstream>
#include <string>

class WindowManager {
    public:
        WindowManager(int sizeX, int sizeY);
        virtual ~WindowManager();
        static void errorCallback(int error, const char* description);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow  *window;
        GLuint      shaderProgram;
        GLint       ulocModelview;
        GLint       ulocProject;
        GLfloat      projectionMatrix[16];
        GLfloat      modelViewMatrix[16];
        GLfloat     viewAngle;
        GLfloat     aspectRatio;
        GLfloat     zFar;
        GLfloat     zNear;
        int         frame;
        int         iter;
        double      dt;
        double      lastUpdateTime;

        GLuint      makeShaderProgram(const std::string *vsText, const std::string *fsText);
        GLuint      makeShader(GLenum type, const std::string *text);
        MeshManager *meshManager;
        std::string *getShader(std::string filename) const; 

        void        initMatrix( void );
};

static const char* vertexShaderText =
"#version 150\n"
"uniform mat4 project;\n"
"uniform mat4 modelview;\n"
"in float x;\n"
"in float y;\n"
"in float z;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = project * modelview * vec4(x, y, z, 1.0);\n"
"}\n";

static const char* fragmentShaderText =
"#version 150\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = vec4(0.2, 1.0, 0.2, 1.0); \n"
"}\n";

#endif
