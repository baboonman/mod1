#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <cmath>
# include "OpenGLInc.hpp"

typedef struct					s_vec
{
	GLfloat						x;
	GLfloat						y;
	GLfloat						z;
}								t_vec;

typedef struct					s_vec2
{
	GLfloat						u;
	GLfloat						v;
}								t_vec2;

typedef struct					s_vecf
{
	float						x;
	float						y;
	float						z;
}								t_vecf;

t_vec							normalize(t_vec v);
t_vec							addVec(t_vec v1, t_vec v2);
t_vec							cross(t_vec v1, t_vec v2);
t_vec							add(t_vec v1, t_vec v2);
t_vec							sub(t_vec v1, t_vec v2);

t_vecf							normalize(t_vecf v);
t_vecf							add(t_vecf v1, t_vecf v2);
t_vecf							sub(t_vecf v1, t_vecf v2);
t_vecf							scale(t_vecf v, float s);
t_vecf							cross(t_vecf v1, t_vecf v2);
float							dot(t_vecf v1, t_vecf v2);
t_vecf							inverse(t_vecf v);
t_vecf							translate(t_vecf point, t_vecf trVec);

#endif
