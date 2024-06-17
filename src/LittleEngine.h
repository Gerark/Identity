#pragma once

#include "EngineImpl.h"

#include "imgui/imgui.h"

class LittleEngine {
public:
	void init();
	void run(const UpdateCallback& updateCallback);

	ImFont* getFontBySize(unsigned int size);

private:
	void _initSDL();
	void _initImGui();
	bool _internalLoop();
	void _cleanup();
	void _setImGuiStyle();
	void _addFont(std::string_view path, std::initializer_list<unsigned int> sizes, unsigned int defaultSize);

	EngineImpl _engine;
	SDL_Window* _window;
	SDL_GLContext _context;
	UpdateCallback _updateCallback;
	std::unordered_map<unsigned int, ImFont*> _fonts;
};
