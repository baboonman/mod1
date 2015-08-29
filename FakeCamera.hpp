#ifndef FAKECAMERA_HPP
# define FAKECAMERA_HPP

# include "CameraControl.hpp"

class				FakeCamera : public CameraControl
{
	public:
					FakeCamera(float x, float y, float z);
		void		control(int key, int action, int mods);

	protected:
		float		_x;
		float		_y;
		float		_z;
};

#endif
