#pragma once

#include "EngineImpl.h"

#include "imgui/imgui.h"

#include <unordered_map>
#include <vector>
#include <string_view>

struct TextureInfo {
	GLuint id{};
	int width{};
	int height{};
	int channels{};

	void* toImGui() {
		return reinterpret_cast<void*>(static_cast<intptr_t>(id));
	}

	ImVec2 toImGuiSize() {
		return ImVec2(static_cast<float>(width), static_cast<float>(height));
	}
};

/*
* The LittleEngine class is mostly a little wrapper around openGL function call and ImGui.
* It's the only entry point for the use of any openGL layering. It's not pretending to be advanced at all.
*/
class LittleEngine {
public:
	void init();
	void run(const UpdateCallback& updateCallback);

	ImFont* getFontBySize(unsigned int size);
	void openUrl(std::string_view url);

	TextureInfo loadTexture(std::string_view path);

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
	std::vector<GLuint> _textures;
};
