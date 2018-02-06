#pragma once

#include "Graphics.h"
#include "Input.h"

namespace Framework
{
	class Game abstract
	{
	public:
		Graphics graphics;
		Input input;

		int run();

		virtual void start() {};
		virtual void update() {};
		virtual void draw() {};
		virtual void stop() {};
		virtual void onMouseMove(int, int) {};
		virtual void onMouseWheel(int) {};

	private:
		static const int MILLISECONDS_YIELD_ = 10;

		bool isRunning_ = false;
		bool isActive_ = true;

		void gameLoop_();
	};
}
