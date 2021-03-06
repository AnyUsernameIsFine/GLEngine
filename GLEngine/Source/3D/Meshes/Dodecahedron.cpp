#include "Dodecahedron.h"

namespace GLEngine
{
	const std::vector<Mesh::Vertex>& DodecahedronMesh::getVertices() const
	{
		return vertices;
	}

	const std::vector<GLushort>& DodecahedronMesh::getIndices() const
	{
		return indices;
	}

	void DodecahedronMesh::initialize()
	{
		Utility::StaticConstructor<&DodecahedronMesh::initialize>::c;

		std::vector<glm::vec3> positions;

		float h = (sqrtf(5) - 1) / 2;
		float onePlusH = 1 + h;
		float oneMinusH2 = 1 - h * h;

		float a = (2 + onePlusH) / 5;
		float b = (2 + 2 * onePlusH + oneMinusH2) / 5;
		float scale = 1 / sqrtf(a * a + b * b);

		for (int i = -1; i <= +1; i += 2) {
			for (int j = -1; j <= +1; j += 2) {
				for (int k = -1; k <= +1; k += 2) {
					positions.emplace_back(glm::vec3(i, j, k) * scale);
				}
				auto permutations = getEvenPermutationsOfVertex(glm::vec3(i * onePlusH, j * oneMinusH2, 0) * scale);
				positions.insert(positions.end(), permutations.begin(), permutations.end());
			}
		}

		std::vector<std::vector<int>> faces = {
			{ 0, 2, 7, 5, 4 },
			{ 0, 3, 8, 1, 2 },
			{ 0, 4, 9, 10, 3 },
			{ 1, 8, 11, 19, 14 },
			{ 1, 14, 6, 7, 2 },
			{ 3, 10, 12, 11, 8 },
			{ 4, 5, 13, 15, 9 },
			{ 5, 7, 6, 18, 13 },
			{ 6, 14, 19, 16, 18 },
			{ 9, 15, 17, 12, 10 },
			{ 11, 12, 17, 16, 19 },
			{ 13, 18, 16, 17, 15 },
		};

		CreatedMesh mesh = createMesh(positions, faces);

		vertices = mesh.vertices;
		indices = mesh.indices;
	}

	std::vector<Mesh::Vertex> DodecahedronMesh::vertices;
	std::vector<GLushort> DodecahedronMesh::indices;
}
