#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Game
{
	const bool Camera::isPerspective() const
	{
		return perspective_;
	}

	const float Camera::getFieldOfView() const
	{
		return fov_;
	}

	void Camera::setPerspective(bool perspective)
	{
		perspective_ = perspective;
	}

	void Camera::setAspectRatio(float ratio)
	{
		ratio_ = ratio;
	}

	void Camera::setFieldOfView(float fov)
	{
		fov_ = fmin(fmax(10, fov), 170);
	}

	void Camera::setSize(int size)
	{
		size_ = size;
	}

	const glm::mat4 Camera::getViewMatrix() const
	{
		return glm::translate(glm::mat4_cast(transform.getOrientationQuaternion()), -transform.getPosition());
	}

	const glm::mat4 Camera::getProjectionMatrix() const
	{
		if (perspective_) {
			return glm::perspective(glm::radians(fov_), ratio_, near_, far_);
		}
		else {
			float halfHeight = size_ * 0.5f;
			float halfWidth = halfHeight * ratio_;
			return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near_, far_);
		}
	}
}
