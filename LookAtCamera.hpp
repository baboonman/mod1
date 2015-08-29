#ifndef LOOKATCAMERA_HPP
# define LOOKATCAMERA_HPP

# include "CameraControl.hpp"

typedef struct				s_vecf
{
	float					x;
	float					y;
	float					z;
}							t_vecf;

class				LookAtCamera : public CameraControl
{
	public:
					LookAtCamera(t_vecf eyePos, t_vecf targetPos, t_vecf upVector);
		void		control(int key, int action, int mods);

	private:
		Matrix		calculateMatrix();

	protected:
		t_vecf		_eyePos;
		t_vecf		_targetPos;
		t_vecf		_upVector;
};

#endif
