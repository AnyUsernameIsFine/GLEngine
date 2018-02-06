#pragma once

#include <SDL.h>

namespace Framework
{
	class Input
	{
	public:
		Input();
		const bool isKeyDown(SDL_Keycode key) const;

	private:
		friend class Game;

		const Uint8* keyboardState_;

		void processEvent_(SDL_Event event);
	};
}
