#include "OpenGLManager.hpp"
#include "FakeCamera.hpp"
#include "LookAtCamera.hpp"
#include "FreeCamera.hpp"

#include <stddef.h>
#include <unistd.h>

int					main(int ac, char **av)
{
//	OpenGLManager	oGlMan(1280,720, "Biatch");
	OpenGLManager	oGlMan(1920,1050, "Biatch");
	t_vecf			eye = {0.0f, 0.0f, -0.5f};
	t_vecf			target = {0.0f, 0.0f, 10.0f};
	t_vecf			up = {0.0f, 1.0f, 0.0f};
	CameraControl	*camera = new FakeCamera(0.0f, 0.0f, -0.5f);
	CameraControl	*camera2 = new LookAtCamera(eye, target, up);
	CameraControl	*camera3 = new FreeCamera(eye, 0.0f, 0.0f);
	std::string		filename = "resources/Suzanne.obj";

	if (ac == 2)
		filename = av[1];
	Mesh			mesh(filename);
	std::cout << "Parsing done" << std::endl;
	oGlMan.initShader("shader/shad.vert", "shader/shad.frag");
	std::cout << "Shader Program created." << std::endl;
	mesh.initMeshIndices(oGlMan.getShaderProgram());
	std::cout << "Vbo created." << std::endl;
	oGlMan.run(camera3, mesh);
	return 0;
}
