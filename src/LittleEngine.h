#pragma once

#include "EngineImpl.h"

class LittleEngine {
public:
	void init();
	void run(const UpdateCallback& updateCallback);

private:
	void _initSDL();
	void _initImGui();
	bool _internalLoop();
	void _cleanup();

	EngineImpl _engine;
	SDL_Window* _window;
	SDL_GLContext _context;
	UpdateCallback _updateCallback;
};
