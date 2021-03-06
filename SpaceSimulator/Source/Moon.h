#pragma once

#include <glm\gtc\random.hpp>
#include "CoordinateSystem.h"

namespace SpaceSimulator
{
	class Moon : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAX_RADIUS;
		static const glm::vec4 COLOR;

		Moon(CoordinateSystem* parent, float radius);
		float getScale() const;
		const glm::vec4& getColor() const;
		float getCameraNearPlane() const;
	};
}
