#include "HeightmapGenerator.hpp"
#include <iostream>

HeightmapGenerator::HeightmapGenerator()
{
	this->_gridSize = 100;
	this->_maxHeight = 50.f;
	this->createTerrain(true);
}

HeightmapGenerator::~HeightmapGenerator()
{
}

void							HeightmapGenerator::exportMeshData(std::vector<t_vec> & vertices, std::vector<t_vec> & normals, std::vector<GLuint> & indices)
{
	vertices = this->_vertex;
	normals = this->_vertexN;
	indices = this->_indices;
}

void							HeightmapGenerator::createTerrain(bool top)
{
	std::vector<t_vec>			tops;
	GRID						map(this->_gridSize, std::vector<float>(this->_gridSize));
	t_vec						t1 = {25, 25, 25};
	t_vec						t2 = {50, 50, 50};
	t_vec						t3 = {75, 75, 75};
	ProcMap						pMap(this->_gridSize);

	if (top)
	{
		tops.push_back(t1);
		tops.push_back(t2);
		tops.push_back(t3);
	}
	
//	createMap(map, this->_gridSize, this->_maxHeight, tops, 0.7f);
	pMap.createMap();
	pMap.scaleMap(this->_maxHeight);
	map = pMap.getMap();
	computeVert(map, this->_gridSize);
	computeNorm();
	computeIndices();
}

float					HeightmapGenerator::calcUp(int i, int j, t_vec top)
{
	float		n;

	n = sqrt(pow(static_cast<float>(i) - top.x, 2.0) + pow(static_cast<float>(j) - top.y, 2.0));
	n = n / THRESHOLD;
	if (n > 1.0f)
		n = 1.0f;
	n = 1 - n;
//	printf("n: %f\n=================\n", n);
	return (n);
}
/*
void					HeightmapGenerator::createMap(GRID &map, int gridSize, float height, std::vector<t_vec> tops, float t)
{
	int					octaves	= 6;
	float				n = 0, m, b;
	float				nCoef = 0.f;

	for (int i = 0 ; i < gridSize ; i++)
	{
		for (int j = 0 ; j < gridSize ; j++)
		{
			n = 0.0f;
			for (size_t it = 0 ; it < tops.size() ; it++) {
				m = calcUp(i, j, tops[it]);
				nCoef = m * 0.5f;
				if (m < 0.1)
					m = 0.1;
//				nCoef = m + 0.5;
				b = Noise::sound3D(i, j, t, octaves, 0.5);
//				m = b * nCoef + sin(m * M_PI_2) * sin(m * M_PI_2);
				m = b + nCoef;
				n = fmax(n, m);
			}
			if (tops.size() == 0)
				map[i][j] = Noise::sound3D(i, j, t, octaves, 0.5) * height;
			else
				map[i][j] = n * height;
		}
	}
}
*/
void					HeightmapGenerator::computeVert(GRID &map, int gridSize)
{
	for (int i = 0 ; i < gridSize ; i++)
	{
		for (int j = 0 ; j < gridSize ; j++)
		{
			t_vec v = {static_cast<GLfloat>(j - gridSize/2), map[i][j], static_cast<GLfloat>(gridSize/2 - i)};
			this->_vertex.push_back(v);
		}
	}
}

void					HeightmapGenerator::computeIndices()
{
	for (int i = 0 ; i < this->_gridSize - 1 ; i++)
	{
		for (int j = 0 ; j < this->_gridSize - 1 ; j++)
		{
			this->_indices.push_back(i * this->_gridSize + j);
			this->_indices.push_back(i * this->_gridSize + j + 1);
			this->_indices.push_back((i + 1) * this->_gridSize + j);
			this->_indices.push_back((i + 1) * this->_gridSize + j);
			this->_indices.push_back((i + 1) * this->_gridSize + j + 1);
			this->_indices.push_back(i * this->_gridSize + j + 1);
		}
	}
}

t_vec					HeightmapGenerator::getNorm(GLuint center, GLuint first, GLuint second)
{
	t_vec				v1;
	t_vec				v2;
	t_vec				vres;

	v1 = sub(this->_vertex[first], this->_vertex[center]);
	v2 = sub(this->_vertex[second], this->_vertex[center]);
	vres = normalize(cross(v1, v2));
	return vres;
}

void					HeightmapGenerator::computeNorm()
{
	for (int i = 0 ; i < this->_gridSize ; i++)
	{
		for (int j = 0 ; j < this->_gridSize ; j++)
		{
			if (i > 0 && i < this->_gridSize - 1 && j > 0 && j < this->_gridSize - 1)
			{
				t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
				t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
				this->_vertexN.push_back(normalize(add(d_r, u_l)));
			}

			else if (i == 0)
			{
				if (j == 0)
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					this->_vertexN.push_back(normalize(d_r));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					this->_vertexN.push_back(normalize(d_l));
				}
				else
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					this->_vertexN.push_back(normalize(add(d_r, d_l)));
				}
			}

			else if (i == this->_gridSize - 1)
			{
				if (j == 0)
				{
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexN.push_back(normalize(u_r));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					this->_vertexN.push_back(normalize(u_l));
				}
				else
				{
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexN.push_back(normalize(add(u_l, u_r)));
				}
			}

			else if (i > 0 && i < this->_gridSize - 1)
			{
				if (j == 0)
				{
					t_vec d_r = getNorm(i * this->_gridSize + j, i * this->_gridSize + j + 1, (i + 1) * this->_gridSize + j);
					t_vec u_r = getNorm(i * this->_gridSize + j, (i - 1) * this->_gridSize + j, i * this->_gridSize + j + 1);
					this->_vertexN.push_back(normalize(add(u_r, d_r)));
				}
				else if (j == this->_gridSize - 1)
				{
					t_vec d_l = getNorm(i * this->_gridSize + j, (i + 1) * this->_gridSize + j, i * this->_gridSize + j - 1);
					t_vec u_l = getNorm(i * this->_gridSize + j, i * this->_gridSize + j - 1, (i - 1) * this->_gridSize + j);
					this->_vertexN.push_back(normalize(add(d_l, u_l)));
				}
			}
		}
	}
}
