#include "Vector.hpp"
#include <iostream>

Vector::Vector() : x(0), y(0), z(0)
{
}

Vector::Vector(float pX, float pY, float pZ) : x(pX), y(pY), z(pZ)
{
}

void            Vector::fromPoint(float *points) {
    this->x = points[3] - points[0];
    this->y = points[4] - points[1];
    this->z = points[5] - points[2];
}

float 			Vector::getLength() const
{
	return std::sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

Vector const	Vector::getUnit()
{
	const float	epsilon = 1e-6;
	float			mag = this->getLength();
	Vector			vector;

	if (mag < epsilon)
	{
		throw new std::exception;
	}

	vector.x /= mag;
	vector.y /= mag;
	vector.z /= mag;
	return (vector);
}

void		Vector::printVector()
{
	std::cout << "x: " << this->x << ", y: " << this->y << ", z: " << this->z;
}

float		Vector::calcDistPoint(Vector const &p2) const
{
	float	xSum = this->x + p2.x;
	float	ySum = this->y + p2.y;
	float	zSum = this->z + p2.z;

	return sqrt(xSum * xSum + ySum * ySum + zSum * zSum);
}

float 		Vector::dot(Vector const &v)
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vector          Vector::operator* (const Vector &v2) {
    Vector   result;
    result.x = this->y * v2.z - this->z * v2.y;
    result.y = this->z * v2.x - this->x * v2.z;
    result.z = this->x * v2.y - this->y * v2.x;
    return result;
}

bool		Vector::operator==(const Vector& rhs) const
{
	if (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z)
	{
		return (true);
	}
	return (false);
}

bool		Vector::operator!=(const Vector& rhs) const
{
	if (this->x != rhs.x || this->y != rhs.y || this->z != rhs.z)
	{
		return (true);
	}
	return (false);
}
