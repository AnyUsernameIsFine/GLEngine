#include "Moon.h"

namespace SpaceSimulator
{
#ifdef UNIVERSE_SCALE
	const float Moon::SCALE = 1;
	const float Moon::MAX_RADIUS = Int64{ 1 } << 62;
#else
	// A little over 0.06 millimeters per unit.
	const float Moon::SCALE = 1.0f / (1 << 14);

	// Ganymede, the largest moon in our Solar System, which is in
	// orbit around Jupiter has a radius of over 2634 kilometers.
	// Let's make that fit in about twice for some wiggle room.
	const float Moon::MAX_RADIUS = 1000 / SCALE * 2634 * 2;
#endif
	const glm::vec4 Moon::COLOR = { 0.5, 0, 1, 0.5 };

	Moon::Moon(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		name = "Moon #" + std::to_string(static_cast<UInt>(Random::randInt()));
		setMesh<IcosahedronMesh>();
	}

	float Moon::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Moon::getColor() const
	{
		return COLOR;
	}

	float Moon::getCameraNearPlane() const
	{
		return 1;
	}
}
