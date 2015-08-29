#include "LookAtCamera.hpp"

t_vecf				normalize(t_vecf v)
{
	t_vecf			v2;
	float			len;
	float			e = 10E-6;

	len = v.x * v.x + v.y * v.y + v.z * v.z;
	if (len < e )
		return v;
	len = sqrt(len);
	v2.x = v.x / len;
	v2.y = v.y / len;
	v2.z = v.z / len;
	return v2;
}

t_vecf				sub(t_vecf v1, t_vecf v2)
{
	t_vecf			res;

	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return res;
}

t_vecf				cross(t_vecf v1, t_vecf v2)
{
	t_vecf			res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return res;
}

float				dot(t_vecf v1, t_vecf v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vecf				inverse(t_vecf v)
{
	t_vecf			res;

	res.x = v.y * -1;
	res.y = v.z * -1;
	res.z = v.x * -1;
	return res;
}

LookAtCamera::LookAtCamera(t_vecf eyePos, t_vecf targetPos, t_vecf upVector)
: _eyePos(eyePos), _targetPos(targetPos), _upVector(upVector)
{
	this->_viewMatrix.setMatrix(calculateMatrix());
}

void			LookAtCamera::control(int key, int action, int mods)
{
	float		sens = 0.5f;

	if (action == ACTION_PRESS)
	{
		if (mods == MOD_SHIFT)
		{
			if (key == 32) // KEY_SPACE
				this->_eyePos.z -= sens;
		}
		else
		{
			if (key == 32) // KEY_SPACE
				this->_eyePos.z += sens;
			else if (key == (int)'W')
				this->_eyePos.y += sens;
			else if (key == (int)'S')
				this->_eyePos.y -= sens;
			else if (key == (int)'D')
				this->_eyePos.x += sens;
			else if (key == (int)'A')
				this->_eyePos.x -= sens;
		}
	}
	this->_viewMatrix.setMatrix(calculateMatrix());
}

Matrix			LookAtCamera::calculateMatrix()
{
	t_vecf zAxis = normalize(sub(this->_targetPos, this->_eyePos));
	t_vecf xAxis = normalize(cross(this->_upVector, zAxis));
	t_vecf yAxis = cross(zAxis, xAxis);

	Matrix		viewMatrix(xAxis.x, xAxis.y, xAxis.z, 0,
						   yAxis.x, yAxis.y, yAxis.z, 0,
						   zAxis.x, zAxis.y, zAxis.z, 0,
						  -dot( xAxis, _eyePos ), -dot( yAxis, _eyePos ), dot( zAxis, _eyePos ), 1);
	return viewMatrix;
}
