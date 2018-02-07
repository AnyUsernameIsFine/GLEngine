﻿#pragma once

#include "Camera.h"
#include "Transform.h"

#include <Game.h>
#include <GLTexture2D.h>
#include <GLProgram.h>
#include <VertexBufferObject.h>
#include <Error.h>

#include <string>
#include <sstream>
#include <chrono>
#include <iostream>

namespace Game
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		GLProgram * program;
		GLTexture2D* texture1;
		GLTexture2D* texture2;
		GLuint VAO;

		Camera camera;

		Transform cubePositions[10]{
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		Game()
		{
			graphics.window.setTitle("Space Simulator");
			graphics.window.setSize(960, 540);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			graphics.text.setFont("Resources/lucon.ttf", 20);
			graphics.text.setColor(1, 1, 1);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			//camera.usePerspective(false);
			camera.transform.moveZ(3);

			texture1 = new GLTexture2D("Resources/journey.jpg");
			texture2 = new GLTexture2D("Resources/flow.jpg");

			program = new GLProgram("Resources/simple.vert", "Resources/simple.frag");

			program->use();
			program->setUniform("texture1", 0);
			program->setUniform("texture2", 1);

			float vertices[] = {
				-0.5, -0.5, -0.5,	0, 0,
				 0.5, -0.5, -0.5,	1, 0,
				-0.5,  0.5, -0.5,	0, 1,
				 0.5,  0.5, -0.5,	1, 1,
				-0.5, -0.5,  0.5,	0, 0,
				 0.5, -0.5,  0.5,	1, 0,
				-0.5,  0.5,  0.5,	0, 1,
				 0.5,  0.5,  0.5,	1, 1,
			};

			int indices[] = {
				0, 2, 1, 1, 2, 3,
				0, 1, 4, 4, 1, 5,
				0, 4, 2, 2, 4, 6,
				1, 3, 5, 5, 3, 7,
				2, 6, 3, 3, 6, 7,
				4, 5, 6, 6, 5, 7,
			};

			for (int i = 0; i < 10; i++) {
				cubePositions[i].rotate(i * 20.0f, { -1.0f, 0.3f, 0.5f });
			}

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			VertexBufferObject vbo({ 3, 2 }, 8, vertices);

			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		void onKeyDown(SDL_Keycode key)
		{
			if (key == SDLK_p) {
				camera.setPerspective(!camera.isPerspective());
			}
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 10 * graphics.getDeltaSeconds();

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void onMouseWheel(int y)
		{
			camera.setFieldOfView(camera.getFieldOfView() - y * 10);
		}

		void update()
		{
			float cameraSpeed = 5 * graphics.getDeltaSeconds();

			camera.transform.moveX(cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)));
			camera.transform.moveZ(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w)));
		}

		void draw()
		{
			graphics.clearScreen(1, 0, 0.5);

			// OpenGL tutorial
			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = camera.getProjectionMatrix();

			program->use();
			program->setUniform("view", view);
			program->setUniform("projection", projection);
			program->setUniform("mix", (sin(graphics.getTotalSeconds()) + 1) * 0.5f);

			texture1->use(0);
			texture2->use(1);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glBindVertexArray(VAO);

			for (int i = 0; i < 10; i++) {
				cubePositions[i].rotate(graphics.getDeltaSeconds() * 50.0f, { 1.0f, 0.3f, 0.5f });
				glm::mat4 model = cubePositions[i].getModelMatrix();
				program->setUniform("model", model);

				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			}
			//glBindVertexArray(0);
			// end of tutorial

			int fps = (int)round(graphics.getFps());
			graphics.text.draw(0, 0, (std::to_string(fps) + "fps"));
			//graphics.text.draw(0, 0, std::to_wstring(fps) + L"fps\nZażółć gęślą jaźń\nEl veloz murciélago hindú comía feliz cardillo y kiwi.\nLa cigüeña tocaba el saxofón detrás del palenque de paja.");
		}
	};
}
