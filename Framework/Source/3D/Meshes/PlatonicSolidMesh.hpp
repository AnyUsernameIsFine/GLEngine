#pragma once

#include "3D/Mesh.hpp"

namespace Framework
{
	class PlatonicSolidMesh abstract : public Mesh
	{
	protected:
		struct CreatedMesh
		{
			vector<Vertex> vertices;
			vector<GLushort> indices;
		};

		static vector<glm::vec3> evenPermutationsOfVertex(const glm::vec3& v);
		static vector<glm::vec3> oddPermutationsOfVertex(const glm::vec3& v);
		static vector<glm::vec3> evenAndOddPermutationsOfVertex(const glm::vec3& v);
		static CreatedMesh createMesh(const vector<glm::vec3>& positions, const vector<vector<int>>& faces);
	};
}
