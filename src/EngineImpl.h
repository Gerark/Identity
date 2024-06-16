#pragma once

#include <string_view>
#include <functional>

#ifdef __EMSCRIPTEN__

#include <SDL2/SDL.h>
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>

class EngineImpl {
public:
	void setupGLAttributes();
	SDL_Window* createWindow(std::string_view title);
	SDL_GLContext createContext(SDL_Window* window);
	void run(const std::function<void()>& updateCallback);

private:
	static void _staticUpdateCallback():
		static std::function<void()> _updateCallback;
};

std::function<void()> EngineImpl::_updateCallback;

#else

#include <SDL.h>
#include <glad/gl.h>

class EngineImpl {
public:
	void setupGLAttributes();
	SDL_Window* createWindow(std::string_view title);
	SDL_GLContext createContext(SDL_Window* window);
	void run(const std::function<void()>& updateCallback);
};

#endif
