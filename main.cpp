#include "OpenGlManager.hpp"

#include <stddef.h>
#include <unistd.h>

int					main(int ac, char **av)
{
//	OpenglManager	oGlMan(1280,720, "Biatch");
	OpenglManager	oGlMan(1920,1050, "Biatch");
	std::string		filename = "resources/Suzanne.obj";

	if (ac == 2)
		filename = av[1];
	Mesh			mesh(filename);

	std::cout << "Parsing done" << std::endl;

	oGlMan.initShader("shader/shad.vert", "shader/shad.frag");
	std::cout << "Shader Program created." << std::endl;

	mesh.initMeshIndices(oGlMan.getShaderProgram());
	std::cout << "Vbo created." << std::endl;

	oGlMan.run(mesh);

	return 0;
}
