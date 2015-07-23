#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <cmath>
# include <exception>
# include "Matrix.hpp"

class Vector {

	public:
		Vector();
		Vector(float pX, float pY, float pZ);
		Vector Unit() const;
		Vector const	getUnit();
		float 			dot(Vector const &v);
		void			applyMatrice(Matrix &m);
		void			applyProjectionMatrice(Matrix &m, int width, int height);
		void			printVector();
		float			calcDistPoint(Vector const &p2) const;
        void            fromPoint(float *points);
		bool			operator==(const Vector& rhs) const;
		bool			operator!=(const Vector& rhs) const;
        Vector          operator* (const Vector &v2);

		float			x;
		float			y;
		float			z;
		float			getLength() const;
};

#endif
