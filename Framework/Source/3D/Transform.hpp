#pragma once

#include "Vector3.hpp"

#include <glm\gtc\quaternion.hpp>

#define USE_POTENTIAL_SPEEDUP

namespace Framework
{
	template<typename T>
	class TransformType
	{
	public:
		TransformType(
			const Vector3Type<T>& position = { 0, 0, 0 },
			const glm::vec3& eulerAngles = { 0, 0, 0 },
			const glm::vec3& scale = { 1, 1, 1 }
		);
		void useModelAxes(bool use = true);
		void setPosition(const Vector3Type<T>& position);
		void setPosition(T x, T y, T z);
		void setOrientation(const glm::quat& orientation);
		void setEulerAngles(const glm::vec3& eulerAngles);
		void setEulerAngles(float yaw, float pitch, float roll);
		void setScale(const glm::vec3& scale);
		void setScale(float x, float y, float z);
		void setScale(float scale);
		const Vector3Type<T>& getPosition() const;
		glm::vec3 getEulerAngles() const;
		const glm::quat& getOrientation() const;
		const glm::vec3& getScale() const;
		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void rotate(float angle, const glm::vec3& axis);
		void yaw(float angle);
		void pitch(float angle);
		void roll(float angle);
		void scale(const glm::vec3& scale);
		void scale(float x, float y, float z);
		void scale(float scale);
		void scaleX(float scale);
		void scaleY(float scale);
		void scaleZ(float scale);
		const glm::mat4& getRotationMatrix();
		const glm::mat4& getModelMatrix();
		const glm::mat4& getModelMatrix(const Vector3Type<T>& cameraPosition);
		const glm::mat4& getModelMatrix(const glm::vec3& cameraPosition);

	private:
		Vector3Type<T> position_;
		glm::quat orientation_;
		glm::vec3 scale_;
		bool useModelAxes_ = true;

		bool isRotationMatrixValid_;
		glm::mat4 rotationMatrix_;
		bool isModelMatrixValid_;
		glm::mat4 modelMatrix_;
#ifdef USE_POTENTIAL_SPEEDUP
		glm::vec3 modelMatrixPosition_;
		glm::vec3 modelMatrixPositionRotated_;
#else
		glm::vec3 translationMatrixPosition_;
		glm::mat4 translationMatrix_ = glm::mat4(1);
		bool isRotationScaleMatrixValid_;
		glm::mat4 rotationScaleMatrix_;
#endif // USE_POTENTIAL_SPEEDUP

		const glm::mat4& getModelMatrix_(const glm::vec3& position);
	};

	typedef TransformType<float> Transform;
}

//#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	template<typename T>
	TransformType<T>::TransformType(const Vector3Type<T>& position, const glm::vec3& eulerAngles, const glm::vec3& scale)
	{
		setPosition(position);
		setEulerAngles(eulerAngles);
		setScale(scale);
	}

	template<typename T>
	void TransformType<T>::useModelAxes(bool use)
	{
		useModelAxes_ = use;
	}

	template<typename T>
	void TransformType<T>::setPosition(const Vector3Type<T>& position)
	{
		position_ = position;
	}

	template<typename T>
	void TransformType<T>::setPosition(T x, T y, T z)
	{
		setPosition({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::setOrientation(const glm::quat& orientation)
	{
		orientation_ = orientation;

		isRotationMatrixValid_ = false;
#ifdef USE_POTENTIAL_SPEEDUP
		isModelMatrixValid_ = false;
#else
		isRotationScaleMatrixValid_ = false;
#endif // USE_POTENTIAL_SPEEDUP
	}

	template<typename T>
	void TransformType<T>::setEulerAngles(const glm::vec3& eulerAngles)
	{
		setOrientation(glm::quat(glm::radians(eulerAngles)));
	}

	template<typename T>
	void TransformType<T>::setEulerAngles(float yaw, float pitch, float roll)
	{
		setEulerAngles({ yaw, pitch, roll });
	}

	template<typename T>
	void TransformType<T>::setScale(const glm::vec3& scale)
	{
		scale_ = scale;

#ifdef USE_POTENTIAL_SPEEDUP
		isModelMatrixValid_ = false;
#else
		isRotationScaleMatrixValid_ = false;
#endif // USE_POTENTIAL_SPEEDUP
	}

	template<typename T>
	void TransformType<T>::setScale(float x, float y, float z)
	{
		setScale({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::setScale(float scale)
	{
		setScale(scale, scale, scale);
	}

	template<typename T>
	const Vector3Type<T>& TransformType<T>::getPosition() const
	{
		return position_;
	}

	template<typename T>
	const glm::quat& TransformType<T>::getOrientation() const
	{
		return orientation_;
	}

	template<typename T>
	glm::vec3 TransformType<T>::getEulerAngles() const
	{
		return glm::degrees(glm::eulerAngles(orientation_));
	}

	template<typename T>
	const glm::vec3& TransformType<T>::getScale() const
	{
		return scale_;
	}

	template<typename T>
	void TransformType<T>::move(const glm::vec3& vector)
	{
		if (useModelAxes_) {
			position_ += vector * orientation_;
		}
		else {
			position_ += vector;
		}
	}

	template<typename T>
	void TransformType<T>::move(float x, float y, float z)
	{
		move({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::moveX(float distance)
	{
		move(distance, 0, 0);
	}

	template<typename T>
	void TransformType<T>::moveY(float distance)
	{
		move(0, distance, 0);
	}

	template<typename T>
	void TransformType<T>::moveZ(float distance)
	{
		move(0, 0, distance);
	}

	template<typename T>
	void TransformType<T>::rotate(float angle, const glm::vec3& axis)
	{
		glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);

		if (useModelAxes_) {
			setOrientation(glm::normalize(rotation * orientation_));
		}
		else {
			setOrientation(glm::normalize(orientation_ * rotation));
		}
	}

	template<typename T>
	void TransformType<T>::yaw(float angle)
	{
		rotate(angle, { 0, 1, 0 });
	}

	template<typename T>
	void TransformType<T>::pitch(float angle)
	{
		rotate(angle, { 1, 0, 0 });
	}

	template<typename T>
	void TransformType<T>::roll(float angle)
	{
		rotate(angle, { 0, 0, 1 });
	}

	template<typename T>
	void TransformType<T>::scale(const glm::vec3& scale)
	{
		setScale(scale_ * scale);
	}

	template<typename T>
	void TransformType<T>::scale(float x, float y, float z)
	{
		scale({ x, y, z });
	}

	template<typename T>
	void TransformType<T>::scale(float scale)
	{
		scale_ *= scale;
	}

	template<typename T>
	void TransformType<T>::scaleX(float scale)
	{
		scale(scale, 0, 0);
	}

	template<typename T>
	void TransformType<T>::scaleY(float scale)
	{
		scale(0, scale, 0);
	}

	template<typename T>
	void TransformType<T>::scaleZ(float scale)
	{
		scale(0, 0, scale);
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getRotationMatrix()
	{
		if (!isRotationMatrixValid_) {
			rotationMatrix_ = glm::mat4_cast(glm::conjugate(orientation_));
			isRotationMatrixValid_ = true;
		}

		return rotationMatrix_;
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix()
	{
		return getModelMatrix_({ position_.x, position_.y, position_.z });
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix(const Vector3Type<T>& cameraPosition)
	{
		Vector3Type<T> distance = position_ - cameraPosition;

		return getModelMatrix_({ distance.x, distance.y, distance.z });
	}

	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix(const glm::vec3& cameraPosition)
	{
		return getModelMatrix_({
			(float)position_.x - cameraPosition.x,
			(float)position_.y - cameraPosition.y,
			(float)position_.z - cameraPosition.z,
			});
	}

#ifdef USE_POTENTIAL_SPEEDUP
	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix_(const glm::vec3& position)
	{
		if (position != modelMatrixPosition_) {
			modelMatrixPosition_ = position;
			modelMatrixPositionRotated_ = orientation_ * position;

			isModelMatrixValid_ = false;
		}

		if (!isModelMatrixValid_) {
			modelMatrix_ = glm::scale(glm::translate(getRotationMatrix(), modelMatrixPositionRotated_), scale_);
			isModelMatrixValid_ = true;
		}

		return modelMatrix_;
	}
#else
	template<typename T>
	const glm::mat4& TransformType<T>::getModelMatrix_(const glm::vec3& position)
	{
		if (position != translationMatrixPosition_) {
			translationMatrix_ = glm::translate(glm::mat4(1), position);
			translationMatrixPosition_ = position;
		}

		if (!isRotationScaleMatrixValid_) {
			rotationScaleMatrix_ = glm::scale(getRotationMatrix(), scale_);
			isRotationScaleMatrixValid_ = true;
			isModelMatrixValid_ = false;
		}

		if (!isModelMatrixValid_) {
			modelMatrix_ = translationMatrix_ * rotationScaleMatrix_;
			isModelMatrixValid_ = true;
		}

		return modelMatrix_;
	}
#endif // USE_POTENTIAL_SPEEDUP
}
