#ifndef CAMERACONTROL_HPP
# define CAMERACONTROL_HPP

# include "OpenGLMatrix.hpp"

# define	ACTION_RELEASE			0
# define	ACTION_PRESS			1
# define	ACTION_REPEAT			2

# define	MOD_SHIFT				0x0001


class						CameraControl
{
	public:
		OpenGLMatrix		getViewMatrix() const { return this->_viewMatrix; };
		virtual void		control(int key, int action, int mods) = 0;

	protected:
		OpenGLMatrix		_viewMatrix;
};


#endif
