#pragma once

#include "EngineImpl.h"

class LittleEngine {
public:
	void init();
	void run(const std::function<void()>& updateCallback);

private:
	void _initSDL();
	void _initImGui();
	void _internalLoop();

	EngineImpl _engine;
	SDL_Window* _window;
	SDL_GLContext _context;
	std::function<void()> _updateCallback;
};
