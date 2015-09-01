#include "Mesh.hpp"

	const GLfloat bigCube[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

int			Mesh::_i = 0;
Mesh::Mesh()
{
	_mesh = bigCube;
	_id = _i;
	_i++;
//    this->_vertices = new std::vector<GLfloat>;
//    this->_indicesVert = new std::vector<GLuint>;
//    this->_normal = new std::vector<GLfloat>;
}

Mesh::Mesh(const GLfloat *mesh)
{
	_mesh = mesh;
	_id = _i;
	_i++;
	_curInd = 0;
//    this->_vertices = new std::vector<GLfloat>;
//    this->_indicesVert = new std::vector<GLuint>;
//    this->_normal = new std::vector<GLfloat>;
}

Mesh::Mesh(const std::string fileName)
{
	loadOBJ(fileName);
	_id = _i;
	_i++;
	_curInd = 0;
//    this->_vertices = new std::vector<GLfloat>;
//    this->_indicesVert = new std::vector<GLuint>;
//    this->_normal = new std::vector<GLfloat>;
}

Mesh::Mesh(const GLfloat *mesh, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat pos[3])
{
	_curInd = 0;
	_mesh = mesh;
	_id = _i;
	_modelMatrix.scale( scale );
	_modelMatrix.rotateX( rotX );
	_modelMatrix.rotateY( rotY );
	_modelMatrix.rotateZ( rotZ );
	_modelMatrix.translate( pos[0], pos[1], pos[2] );
	_i++;
//    this->_vertices = new std::vector<GLfloat>;
//    this->_indicesVert = new std::vector<GLuint>;
//    this->_normal = new std::vector<GLfloat>;
}

Mesh::Mesh(const std::string fileName, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat pos[3])
{
	_curInd = 0;
	loadOBJ(fileName);
	_id = _i;
	_modelMatrix.scale( scale );
	_modelMatrix.rotateX( rotX );
	_modelMatrix.rotateY( rotY );
	_modelMatrix.rotateZ( rotZ );
	_modelMatrix.translate( pos[0], pos[1], pos[2] );
	_i++;
//    this->_vertices = new std::vector<GLfloat>;
//    this->_indicesVert = new std::vector<GLuint>;
//    this->_normal = new std::vector<GLfloat>;
}

Mesh::~Mesh() {
//    delete this->_vertices;
//   delete this->_indicesVert;
//    delete this->_normal;
}

void						Mesh::initMeshIndices(GLuint program)
{
	GLuint	attrloc;

	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(4, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexOO.size(), &_vertexOO[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * _vertexNOO.size(), &_vertexNOO[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indicesOO.size(), &_indicesOO[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * this->_nbParticles, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(program, "instancePosition");
	glEnableVertexAttribArray(attrloc);
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(attrloc, 1);

	glBindVertexArray(0);
}

void						Mesh::sendPosition(unsigned int size, std::vector<t_vec> position)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vec) * size, &position[0], GL_DYNAMIC_DRAW);
	
}
/*
void						Mesh::initMeshIndicesMAP(GLuint program) const
{
	GLuint vao, vbo[3];
	GLuint	attrloc;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(4, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices->size(), &_vertices[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Position");
	glEnableVertexAttribArray(attrloc);
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _normal->size(), &_normal[0], GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_VertexN");
	glEnableVertexAttribArray(attrloc);
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indicesVert->size(), &_indicesVert[0], GL_STATIC_DRAW);

}

void						Mesh::initMesh(GLuint program) const
{
	GLuint vao, vbo[2];
	GLuint	attrloc;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bigCube), _mesh, GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	attrloc = glGetAttribLocation(program, "in_Color");
	glVertexAttribPointer(attrloc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);
}
*/

void					Mesh::drawMesh() const
{
	glBindVertexArray(this->_vao);
//	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0, this->_nbParticles);
	glBindVertexArray(0);
}

OpenGLMatrix		Mesh::getModelMatrix( void ) const { return _modelMatrix; }
void				Mesh::setModelMatrix( OpenGLMatrix newMatrix ) { _modelMatrix = newMatrix; }

/*
    ex_FragColor = vec4(out_color);
	ex_FragColor = vec4(n * 0.8, n * 0.2, n, 1.0);
	ex_FragColor = vec4((out_color.x + 1) / 2 , (out_color.y + 1) / 2 , (out_color.z + 1) / 2 , 1.0);
	ex_FragColor = vec4(((out_color.x ) / 2 + n) / 2 , ((out_color.y + 1) / 2 + n) / 2 , ((out_color.z + 1) / 2 + n) / 2 , 1.0);

	out_color = vec4(in_Position.x * , in_Position.y, in_Position.z, 1.0);
	out_color = vec4((cos(in_Position.y) + 1) / 2, (sin(in_Position.y) + 1) / 2, (tan(in_Position.z) + 1 ) / 2, 1.0);
	out_color = vec4((cos(in_Position.y) + 1) / 2, (sin(in_Position.y) + 1) / 2, (tan(in_Position.z) + 1 ) / 2, 1.0);
*/
/*
void				Mesh::getFace(std::string face)
{
	GLuint				v_i;
	GLuint				v_n;
	std::string			p;
	std::istringstream iss(face);

	iss >> v_i >> p >> v_n >> p >> v_n;
	_indices.push_back(v_i - 1.0f);
}
*/
void				printV(std::vector< t_vec > v)
{
	std::cout << std::endl;
	for (size_t i = 0 ; i < v.size() ; i++)
	{
		std::cout << i << ":\t" << v[i].x << " " << v[i].y << " " << v[i].z << std::endl;
	}
	std::cout << std::endl;
}

void				printV(std::vector< GLuint > v)
{
	std::cout << std::endl;
	for (size_t i = 0 ; i < v.size() ; i++)
	{
		std::cout << v[i] ;
		if ( (i + 1) % 3 == 0)
			std::cout << "\n";
		else
			std::cout << "\t";
	}
	std::cout << std::endl;
}

void				printT(double t)
{
	int				m, s, ms;

	m = (int)(t / 60000);
	s = (int)(t / 1000) % 60;
	ms = (int)t % 1000; 
	std::cout << "Parsing time: " << m << "m" << s << "s" << ms << "ms." << std::endl;
}

int					isDigit(char c)
{
	return ( c >= '0' && c <= '9' );
}

GLuint				retrieveNb(std::string str, int *i)
{
	std::string		tmp;
	while (str[*i] != '\0' && isDigit(str[*i]))
	{
		tmp += str[*i];
		*i = *i + 1;
	}
	return std::stoi(tmp);
}

void				retrieveInd(std::string face, GLuint &v_i, GLuint &vt_i, GLuint &vn_i)
{
	int				i = 0;

	v_i = retrieveNb(face, &i);
	if (face[i] == '/')
	{
		i++;
		if (face[i] == '/')
		{
			i++;
			vt_i = 0;
			vn_i = retrieveNb(face, &i);
		}
		else if (isDigit(face[i]))
		{
			vt_i = retrieveNb(face, &i);
			if (face[i] == '/')
			{
				i++;
				vn_i = retrieveNb(face, &i);
			}
			else
				vn_i = 0;
		}
	}
	else 
	{
		vt_i = 0;
		vn_i = 0;
	}
}

t_vec				normalize(t_vec v)
{
	t_vec			v2;
	int				len;

	len = v.x * v.x + v.y * v.y + v.z * v.z;
	len = sqrt(len);
	v2.x /= len;
	v2.y /= len;
	v2.z /= len;
	return v2;
}

t_vec				addVec(t_vec v1, t_vec v2)
{
	t_vec			retV;

	retV.x = v1.x + v2.x;
	retV.y = v1.y + v2.y;
	retV.z = v1.z + v2.z;
	return retV;
}

void				Mesh::getFace(std::string face)
{
	GLuint				v_i;
	GLuint				v_n;
	GLuint				v_t;
	std::string			p;
	std::istringstream iss(face);

	auto it = _triplets.find(face);
	if (it != _triplets.end())
	{
		_indicesOO.push_back(_triplets[face]);
		iss >> v_i >> p >> v_n >> p >> v_n;
		_indices.push_back(v_i - 1.0f);
	}
	else
	{
//		iss >> v_i >> p >> v_n >> p >> v_n;
		retrieveInd(face, v_i, v_t, v_n);
		_vertexOO.push_back(_vertex[v_i - 1.0f]);
		_vertexNOO.push_back(_vertexN[v_n - 1.0f]);
		_indicesOO.push_back(_curInd);
		_triplets[face] = _curInd;
		_curInd++;
		_indices.push_back(v_i - 1.0f);
	
	_vecIds.push_back(v_i - 1.0f);
	auto it2 = _oneNorm.find(v_i - 1.0f);
	if (it2 != _oneNorm.end())
		_oneNorm[v_i - 1.0f] = normalize(addVec(_oneNorm[v_i - 1.0f], _vertexN[v_n - 1.0f]));
	else
		_oneNorm[v_i - 1.0f] = _vertexN[v_n - 1.0f];
	
	}
}

void				Mesh::loadOBJ(const std::string fileName)
{
	std::ifstream	infile(fileName);
	std::string		line;
	double			last = glfwGetTime();

	if (!infile.good())
		std::cout << "no file" << std::endl;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string			token;
		iss >> token;
		if (token == "v")
		{
			t_vec		v;
			iss >> v.x >> v.y >> v.z;
			_vertex.push_back(v);
		}
		else if (token == "vt")
		{
			t_vec2		v;
			iss >> v.u >> v.v;
			_vertexT.push_back(v);
		}
		else if (token == "vn")
		{
			t_vec		v;
			iss >> v.x >> v.y >> v.z;
			_vertexN.push_back(v);
		}
		else if (token == "f")
		{
			std::string		f_v;
			iss >> f_v;
			getFace(f_v);	
			iss >> f_v;
			getFace(f_v);	
			iss >> f_v;
			getFace(f_v);	
		}
	 }
/*
	std::cout << "vertex: "  ; printV(_vertex) ; std::cout << std::endl;
	std::cout << "vertexN: " ; printV(_vertexN) ; std::cout << std::endl;
	std::cout << "indices: " ; printV(_indices) ; std::cout << std::endl;
	std::cout << "#########" << std::endl;
	std::cout << "vertexOO: " ; printV(_vertexOO) ; std::cout << std::endl;
	std::cout << "vertexNOO: " ; printV(_vertexNOO) ; std::cout << std::endl;
	std::cout << "indicesOO: " ; printV(_indicesOO) ; std::cout << std::endl;
*/
//	std::cout << "Parsing Time: " << glfwGetTime() - last << "s." << std::endl;
	rearrange();
	printT((glfwGetTime() - last) * 1000);
}

void				Mesh::rearrange()
{
	size_t			i = 0;

	while (i < _vertexOO.size())
	{
		_vertexNOO[i] = _oneNorm[_vecIds[i]];	
		i++;
	}
	
}

GLuint         *Mesh::getVBO( void ) {
    return this->_vbo;
}

void                    Mesh::setNbParticles( int n ) {
    this->_nbParticles = n;
}
