#include <stddef.h>
#include <unistd.h>

#include "OpenGLManager.hpp"
#include "FakeCamera.hpp"
#include "LookAtCamera.hpp"
#include "FreeCamera.hpp"

int					createScene(OpenGLScene *scene, std::string filename)
{
//	int				wID;
	int				mID;
	int				pID;
	int				oID;

//	wID = scene->addShaderProg("shader/water.vert", "shader/lightning.frag");
//	mID = scene->addShaderProg("shader/elevation.vert", "shader/lightning.frag");
//	pID = scene->addShaderProg("shader/particles.vert", "shader/lightning.frag");
	oID = scene->addShaderProg("shader/particles.vert", "shader/lightning.frag");
	if (oID == -1 /* || wID == -1 */)
		return (-1);
	scene->addMesh(MESH_OBJ, filename, oID);
	return (1);
}

int					main(int ac, char **av)
{
	OpenGLManager	oGlMan(800, 800, "Biatch");
//	OpenGLManager	oGlMan(1920,1050, "Biatch toi meme");
	t_vecf			eye = {0.0f, 75.0f, -120.0f};
	CameraControl	*camera3 = new FreeCamera(eye, 0.4f, 0.0f);
	std::string		filename = "resources/Suzanne.obj";
	OpenGLScene		*scene = new OpenGLScene;
	oGlMan.setNbParticles(10000);

	if (ac == 2)
		filename = av[1];

	if (createScene(scene, filename))
	    oGlMan.run(camera3, scene->_meshes[0][0]);
/*
	if (createScene(scene, filename))
		oGlMan.run(camera3, scene);
	else
		std::cout << "Scene wasn't set, error occured." << std::endl;*/
	return 0;
}
