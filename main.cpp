#include <stddef.h>
#include <unistd.h>

#include "OpenGLManager.hpp"
#include "FakeCamera.hpp"
#include "LookAtCamera.hpp"
#include "FreeCamera.hpp"

int					createScene(OpenGLScene *scene)
{
	int				wID;

	wID = scene->addShaderProg("shader/shad.vert", "shader/shad.frag");
	if (wID == -1)
		return (-1);
	scene->addMesh(MESH_WATER, wID);
	return (1);
}

int					main(int ac, char **av)
{
	OpenGLManager	oGlMan(800, 800, "Biatch");
//	OpenGLManager	oGlMan(1920,1050, "Biatch toi meme");
	t_vecf			eye = {0.0f, 75.0f, -120.0f};
	CameraControl	*camera3 = new FreeCamera(eye, 0.4f, 0.0f);
	std::string		filename = "resources/Suzanne.obj";
//	int             nbParticles = 10000;
	OpenGLScene		*scene = new OpenGLScene;

//	if (ac == 2)
//		filename = av[1];
//	Mesh			mesh(filename);
	Mesh			mesh;
//	mesh.setNbParticles(nbParticles);
//	oGlMan.setNbParticles(nbParticles);
//	std::cout << "Parsing done" << std::endl;

//	oGlMan.initShader("shader/shad.vert", "shader/shad.gs", "shader/shad.frag");
	oGlMan.initShader("shader/shad.vert", "shader/shad.frag");
//	std::cout << "Shader Program created." << std::endl;
	mesh.initMeshIndices(oGlMan.getShaderProgram());
//	std::cout << "Vbo created." << std::endl;
//	oGlMan.run(camera3, mesh);

	if (createScene(scene))
		oGlMan.run(camera3, scene);
	else
	{
		std::cout << "Scene wasn't set, error occured." << std::endl;
		oGlMan.run(camera3, mesh);
	}
	return 0;
}
