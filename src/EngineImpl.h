#pragma once

#include <string_view>
#include <functional>

using UpdateCallback = std::function<bool()>;
using ShutdownCallback = std::function<void()>;

#ifdef __EMSCRIPTEN__

#include <SDL2/SDL.h>
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>

class EngineImpl {
public:
	void setupGLAttributes();
	SDL_Window* createWindow(std::string_view title);
	SDL_GLContext createContext(SDL_Window* window);
	void run(const UpdateCallback& updateCallback, const ShutdownCallback& shutdownCallback);
	void openUrl(std::string_view url);

private:
	static void _staticUpdateCallback();
	static UpdateCallback _updateCallback;
	static ShutdownCallback _shutdownCallback;
};

#else

#include <SDL.h>
#include <glad/gl.h>

class EngineImpl {
public:
	void setupGLAttributes();
	SDL_Window* createWindow(std::string_view title);
	SDL_GLContext createContext(SDL_Window* window);
	void run(const UpdateCallback& updateCallback, const ShutdownCallback& shutdownCallback);
	void openUrl(std::string_view url);
};

#endif
