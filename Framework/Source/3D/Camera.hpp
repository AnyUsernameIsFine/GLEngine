#pragma once

#include "Transform.hpp"

namespace Framework
{
	template<typename T>
	class CameraType
	{
	public:
		TransformType<T> transform;

		bool isPerspective() const;
		float getFieldOfView() const;
		float getAspectRatio() const;
		void setPerspective(bool perspective = true);
		void setFieldOfView(float fov);
		void setAspectRatio(float ratio);
		void setClippingPlanes(float near, float far);
		void setSize(float size);
		glm::mat4 getViewMatrix(bool rotationOnly = false) const;
		glm::mat4 getProjectionMatrix() const;

	private:
		bool perspective_ = true;
		float fov_ = 60.0f;
		float ratio_ = 1.0f;
		float near_ = 0.1f;
		float far_ = 100.0f;
		float size_ = 10.0f;
	};

	typedef CameraType<float> Camera;
}

//#include "Camera.h"

namespace Framework
{
	template<typename T>
	bool CameraType<T>::isPerspective() const
	{
		return perspective_;
	}

	template<typename T>
	float CameraType<T>::getFieldOfView() const
	{
		return fov_;
	}

	template<typename T>
	float CameraType<T>::getAspectRatio() const
	{
		return ratio_;
	}

	template<typename T>
	void CameraType<T>::setPerspective(bool perspective)
	{
		perspective_ = perspective;
	}

	template<typename T>
	void CameraType<T>::setAspectRatio(float ratio)
	{
		ratio_ = ratio;
	}

	template<typename T>
	void CameraType<T>::setFieldOfView(float fov)
	{
		fov_ = fov;
	}

	template<typename T>
	void CameraType<T>::setClippingPlanes(float near, float far)
	{
		near_ = near;
		far_ = far;
	}

	template<typename T>
	void CameraType<T>::setSize(float size)
	{
		size_ = size;
	}

	template<typename T>
	glm::mat4 CameraType<T>::getViewMatrix(bool rotationOnly) const
	{
		glm::mat4 rotation = glm::mat4_cast(transform.getOrientation());

		if (rotationOnly) {
			return rotation;
		}
		else {
			return glm::translate(rotation, -transform.getPosition().toVec3());
		}
	}

	template<typename T>
	glm::mat4 CameraType<T>::getProjectionMatrix() const
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
