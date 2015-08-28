#include "Mesh.hpp"
//#include "MapMesh.hpp"
//#include "OpenCL.hpp"

#include <stddef.h>
#include <unistd.h>

#define FROMFPS(X)		( 1 / X )
#define TOFPS(X)		( 1 / X )

#define AN_INT			FROMFPS(3)


std::vector<t_vec>	everywhere(unsigned int size, float step)
{
	std::vector<t_vec>	vec;
	t_vec				v;

	for (unsigned int i  = 0 ; i < size ; i++)
	{
		v.x = i * step;
		v.y = sin(v.x);
		v.z = 2.0f * v.x;
		vec.push_back(v);
	}
	return vec;
}

float				giveMeB(int step, float m)
{
	int				b;
	static int		i = 0;

	if (i >= 0 && i < step)
	{
		b = i;
		i++;
	}
	else
	{
		b = 2 * step - i;
		i++;
	}
	i = i % (2 * step);
	return b * m;
}

int					main(int ac, char **av)
{
//	OpenglManager	oGlMan(1280,720, "Biatch");
	OpenglManager	oGlMan(1920,1050, "Biatch");
	GLuint			prog;
	OpenGlMatrix	modelMat;
	OpenGlMatrix	viewMat;
	OpenGlMatrix	modelViewMat;
	std::string		filename = "resources/Suzanne.obj";
	t_user_ptr		userPtr;
	float			an = 0.0f;
	float			bn = 0.0f;
	double			before;
	double			after;
	double			an_mod;
	double			fps = 0.0;
	std::vector<t_vec>		position;
//	OpenCL			*_openCL;

	if (ac == 2)
		filename = av[1];
	Mesh			mesh(filename);
//	MapMesh			map(300, 300, 0.4);

	oGlMan.addShader(GL_VERTEX_SHADER, "shader/shad.vert");
	oGlMan.addShader(GL_FRAGMENT_SHADER ,"shader/shad.frag");
	if (! oGlMan.createProgram())
	{
		return (0);
	}


	prog = oGlMan.getProgram();
    glUseProgram(prog);
    
	std::cout << "Parsing done" << std::endl;

	mesh.initMeshIndices(prog);
//	map.initMeshIndices(prog);

//	t_vec	vpo; vpo.x = 10.0f; vpo.y = 10.0f; vpo.z = 10.0f;
//	position.push_back(vpo);
	position = everywhere(32, M_PI / 2);

	mesh.sendPosition(32, position);

//    _openCL = new OpenCL(this->getWaterVBO(), this->meshManager->getSizeWaterVBO());

	std::cout << "Vbo creating" << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	oGlMan.createProjectionMatrix();
	viewMat = oGlMan.getModelViewMatrix();
	modelMat = mesh.getModelMatrix();
//	modelMat = map.getModelMatrix();

	viewMat.translate(0.0, 0.0, -5.0);
//	oGlMan.addMatricesToProgram(modelMat, viewMat);

 //   _openCL->initOpenCL();

	userPtr.model = &modelMat;
	userPtr.test = "IEEEE";
	oGlMan.setUserPtr(userPtr);

    while (!oGlMan.shouldClose())
    {
		before = glfwGetTime();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		oGlMan.addMatricesToProgram(modelMat, viewMat, an, bn);

		mesh.drawMesh();
//		map.drawMesh();
		oGlMan.swap();

		glfwPollEvents();

		if (before - an_mod > AN_INT) {
			an += 1;
			an_mod = glfwGetTime();
		}
	//	bn = giveMeB(100, 0.1f);
//		bn += M_PI / 5;
		bn += 0.01f;

		after = glfwGetTime();
		before = 1 / ((after - before)) ;
		if ((before - fps) > 1.0 || (before - fps) < -1.0) {
			fps = before;
			std::cerr << fps << std::endl;
		}
		
//            glFinish();
//            _openCL->executeKernel();
	}

	return 0;
}
