#include "CoordinateSystem.h"

#include <cmath>

namespace Game
{
	CoordinateSystem* CoordinateSystem::getParent() const
	{
		return parent_;
	}

	const std::vector<std::shared_ptr<CoordinateSystem>>& CoordinateSystem::getChildren() const
	{
		return children_;
	}

	const std::string& CoordinateSystem::getName() const
	{
		return name_;
	}

	float CoordinateSystem::getRadius() const
	{
		return radius_;
	}

	void CoordinateSystem::createMesh()
	{
		float vertices[] = {
			-1, -1, -1,		 0,  0, -1,		0, 0,
			-1, -1, -1,		 0, -1,  0,		0, 0,
			-1, -1, -1,		-1,  0,  0,		0, 0,
			1, -1, -1,		 0,  0, -1,		1, 0,
			1, -1, -1,		 0, -1,  0,		1, 0,
			1, -1, -1,		 1,  0,  0,		1, 0,
			-1,  1, -1,		 0,  0, -1,		0, 1,
			-1,  1, -1,		 0,  1,  0,		0, 1,
			-1,  1, -1,		-1,  0,  0,		0, 1,
			1,  1, -1,		 0,  0, -1,		1, 1,
			1,  1, -1,		 0,  1,  0,		1, 1,
			1,  1, -1,		 1,  0,  0,		1, 1,
			-1, -1,  1,		 0,  0,  1,		0, 0,
			-1, -1,  1,		 0, -1,  0,		0, 0,
			-1, -1,  1,		-1,  0,  0,		0, 0,
			1, -1,  1,		 0,  0,  1,		1, 0,
			1, -1,  1,		 0, -1,  0,		1, 0,
			1, -1,  1,		 1,  0,  0,		1, 0,
			-1,  1,  1,		 0,  0,  1,		0, 1,
			-1,  1,  1,		 0,  1,  0,		0, 1,
			-1,  1,  1,		-1,  0,  0,		0, 1,
			1,  1,  1,		 0,  0,  1,		1, 1,
			1,  1,  1,		 0,  1,  0,		1, 1,
			1,  1,  1,		 1,  0,  0,		1, 1,
		};

		unsigned int indices[] = {
			0,  6,  3,  3,  6,  9,
			1,  4, 13, 13,  4, 16,
			2, 14,  8,  8, 14, 20,
			5, 11, 17, 17, 11, 23,
			7, 19, 10, 10, 19, 22,
			12, 15, 18, 18, 15, 21,
		};

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		VertexBufferObject vbo({ 3, 3, 2 }, 24, vertices);

		IndexBufferObject ibo(sizeof(indices) / sizeof(indices[0]), indices);

		glGenBuffers(1, &instanceBuffer_);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);
		glBufferData(GL_ARRAY_BUFFER, MAX_IN_DRAW_LIST_ * sizeof(DrawConfiguration), nullptr, GL_DYNAMIC_DRAW);

		GLsizei vec4Size = sizeof(glm::vec4);

		// mat4
		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(i + 2);
			glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(i * vec4Size + sizeof(CoordinateSystem*)));
			glVertexAttribDivisor(i + 2, 1);
		}
		// mat4
		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(i + 6);
			glVertexAttribPointer(i + 6, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)((i + 4) * vec4Size + sizeof(CoordinateSystem*)));
			glVertexAttribDivisor(i + 6, 1);
		}
		// vec4
		glEnableVertexAttribArray(10);
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(8 * vec4Size + sizeof(CoordinateSystem*)));
		glVertexAttribDivisor(10, 1);
		// float
		glEnableVertexAttribArray(11);
		glVertexAttribPointer(11, 1, GL_FLOAT, GL_FALSE, sizeof(DrawConfiguration), (GLvoid*)(9 * vec4Size + sizeof(CoordinateSystem*)));
		glVertexAttribDivisor(11, 1);

		glBindVertexArray(0);

		shaderProgram_ = new ShaderProgram("Resources/coordinateSystem.vert", "Resources/coordinateSystem.frag");
	}

	void CoordinateSystem::drawWithChildren_(
		std::vector<std::vector<DrawConfiguration>>& toDrawList,
		const std::vector<Camera::CameraHierarchyLevel>& hierarchy,	// list of the camera's positions and rotations relative to all its coordinate system's ancestors from outside in
		int hierarchyIndex,											// which level of the camera hierarchy should we use for inverse rotations
		glm::mat4 rotations,										// matrix of the combined rotations of all this coordinate system's ancestors
		glm::mat4 anotherMatrix,
		glm::vec3 camPos,											// camera position relative to this coordinate system's parent (we have to use floating point precision because this will be used for coordinate systems the camera is outside of)
		bool useHighRes,
		int descendantGenerationsToDraw
	)
	{
		// define the matrix to be used for drawing
		glm::mat4 modelMatrix;

		// only draw descendants if there are any
		bool drawDescendants = !children_.empty();

		DrawConfiguration drawConfiguration;

		// if this coordinate system is in the hierarchy of the camera's coordinate systems
		if (hierarchyIndex < (int)hierarchy.size() - 1 && hierarchy[hierarchyIndex + 1].coordinateSystem == this) {
			hierarchyIndex++;

			// if this coordinate system is an ancestor of the camera's coordinate system,
			// use the camera's rotation relative to this coordinate system's ancestors
			// and draw only one descendant generation
			if (hierarchyIndex < (int)hierarchy.size() - 1) {
				modelMatrix = hierarchy[hierarchyIndex].rotation;
				descendantGenerationsToDraw = 1;
			}

			// if this is the camera's coordinate system, we draw the first two descendant generations
			else {
				modelMatrix = glm::mat4(1);
				descendantGenerationsToDraw = 2;
			}

			// set the rotations for the descendants of this coordinate system
			rotations = modelMatrix;

			// if this is not the universe, translate by the
			// camera's position relative to this coordinate system
			if (parent_) {
				float r = getScale() / parent_->getScale();
				glm::vec3 v = -hierarchy[hierarchyIndex].position.toVec3() * r;
				modelMatrix = glm::translate(modelMatrix, v);
			}

			drawConfiguration = { this, glm::mat4(1), modelMatrix, getColor(), radius_ };
		}

		// if this coordinate system is not in the hierarchy of the camera's coordinate systems
		else {
			// only draw descendants if we're told to
			drawDescendants &= descendantGenerationsToDraw > 0;

			// rotate by this coordinate system's model matrix
			if (useHighRes) {
				useHighRes = false;
				Vector3 highResCamPos = hierarchy[hierarchyIndex].position;
				modelMatrix = transform.getModelMatrix(highResCamPos);

				if (drawDescendants) {
					camPos = (highResCamPos - transform.getPosition()).toVec3();
				}
			}
			else {
				modelMatrix = transform.getModelMatrix(camPos);

				if (drawDescendants) {
					camPos -= transform.getPosition().toVec3();
				}
			}

			if (drawDescendants) {
				// add the current coordinate system's rotation to the combined rotations to use by its descendants
				rotations = glm::mat3(anotherMatrix * modelMatrix); // use the rotation part of the matrix used for drawing

				// calculate the camera position relative to this coordinate system
				camPos = camPos * glm::conjugate(transform.getOrientation()) * parent_->getScale() / getScale();
			}

			drawConfiguration = { this, anotherMatrix, modelMatrix, getColor(), radius_ };
		}

		if (drawDescendants) {
			// create a matrix from the combined rotations of all this coordinate system's ancestors
			// up to but excluding the first shared ancestor with the hierarchy
			// of the camera's coordinate systems (which could be the camera's coordinate system itself)
			// and scale by the relative scale of this coordinate system's direct ancestors
			if (parent_) {
				float r = getScale() / parent_->getScale();
				anotherMatrix = glm::scale(rotations, { r, r, r });
			}
			else {
				anotherMatrix = rotations;
			}

			// draw the coordinate system's descendants
			for (auto& child : children_) {
				child->drawWithChildren_(
					toDrawList,
					hierarchy,
					hierarchyIndex,
					rotations,
					anotherMatrix,
					camPos,
					useHighRes,
					descendantGenerationsToDraw - 1
				);
			}
		}

		// add this coordinate system to the list of coordinate systems to draw
		toDrawList[hierarchyIndex].emplace_back(drawConfiguration);
	}

	void CoordinateSystem::draw_(const std::vector<std::vector<DrawConfiguration>>& toDrawList, const Camera& camera, float deltaSeconds)
	{
		auto hierarchy = camera.getHierarchy();

		float fov = glm::radians(camera.getFieldOfView());
		float ratio = camera.getAspectRatio();

		shaderProgram_->use();
		shaderProgram_->setUniform("view", camera.getViewMatrix(true));

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer_);

		for (int i = 0; i < toDrawList.size(); i++) {
			float near = 0.1f * hierarchy[i].coordinateSystem->getCameraNearPlane();
			float far = near * 100000.0f;
			shaderProgram_->setUniform("projection", glm::perspective(fov, ratio, near, far));

			glClear(GL_DEPTH_BUFFER_BIT);
			glBufferSubData(GL_ARRAY_BUFFER, 0, toDrawList[i].size() * sizeof(DrawConfiguration), &toDrawList[i][0]);
			glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, toDrawList[i].size());
		};
	}

	GLuint CoordinateSystem::vao_;
	GLuint CoordinateSystem::instanceBuffer_;
	ShaderProgram* CoordinateSystem::shaderProgram_;
}
