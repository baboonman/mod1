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
        WindowManager(int sizeX, int sizeY, std::string filename);
        virtual ~WindowManager();
        static void errorCallback(int error, const char* description);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow  *window;
        GLuint      shaderProgram;
        GLint       ulocProject;
        GLint       ulocRot;
        Matrix      projectionMatrix;
        Matrix      modelViewMatrix;
        Matrix      rotationMatrix;
        Matrix      finalProjMatrix;
        GLfloat     viewAngle;
        GLfloat     aspectRatio;
        GLfloat     zFar;
        GLfloat     zNear;
        int         frame;
        int         iter;
        double      dt;
        double      lastUpdateTime;
        int         _sizeX;
        int         _sizeY;
        GLfloat      _rotX;
        GLfloat      _rotY;
        GLfloat      _rotZ;
        Mesh        *_loadedMesh;

        GLuint      makeShaderProgram(const std::string *vsText, const std::string *fsText);
        GLuint      makeShader(GLenum type, const std::string *text);
        MeshManager *meshManager;
        std::string *getShader(std::string filename) const; 

        void        initMatrix( void );
        void        run(void);
};

#endif
