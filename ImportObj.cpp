#include "ImportObj.hpp"

ImportObj::ImportObj()
{
}

ImportObj::ImportObj(ImportObj const & rhs)
{
}

ImportObj::ImportObj(std::string const filename)
{
	this->_curInd = 0;
	loadOBJ(filename);
}

ImportObj::~ImportObj()
{
}


void				ImportObj::printV(std::vector< t_vec > v)
{
	std::cout << std::endl;
	for (size_t i = 0 ; i < v.size() ; i++)
	{
		std::cout << i << ":\t" << v[i].x << " " << v[i].y << " " << v[i].z << std::endl;
	}
	std::cout << std::endl;
}

void				ImportObj::printV(std::vector< GLuint > v)
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

void				ImportObj::printT(double t)
{
	int				m, s, ms;

	m = (int)(t / 60000);
	s = (int)(t / 1000) % 60;
	ms = (int)t % 1000; 
	std::cout << "Parsing time: " << m << "m" << s << "s" << ms << "ms." << std::endl;
}

int					ImportObj::isDigit(char c)
{
	return ( c >= '0' && c <= '9' );
}

GLuint			ImportObj::retrieveNb(std::string str, int *i)
{
	std::string		tmp;
	while (str[*i] != '\0' && isDigit(str[*i]))
	{
		tmp += str[*i];
		*i = *i + 1;
	}
	return std::stoi(tmp);
}

void				ImportObj::retrieveInd(std::string face, GLuint &v_i, GLuint &vt_i, GLuint &vn_i)
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

void				ImportObj::getFace(std::string face)
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

void				ImportObj::loadOBJ(const std::string fileName)
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
	rearrange();
	printT((glfwGetTime() - last) * 1000);
}

void				ImportObj::rearrange()
{
	size_t			i = 0;

	while (i < _vertexOO.size())
	{
		_vertexNOO[i] = _oneNorm[_vecIds[i]];	
		i++;
	}
	
}

void				ImportObj::exportMeshData(std::vector<t_vec> & vertices, std::vector<t_vec> & textures, std::vector<t_vec> & normals, std::vector<GLuint> & indices)
{
	vertices = this->_vertexOO;
	textures = this->_vertexTOO;
	normals = this->_vertexNOO;
	indices = this->_indicesOO;
}
