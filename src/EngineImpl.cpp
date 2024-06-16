#include "EngineImpl.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL_opengles2.h>

UpdateCallback EngineImpl::_updateCallback;
ShutdownCallback EngineImpl::_shutdownCallback;

void EngineImpl::setupGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

SDL_Window* EngineImpl::createWindow(std::string_view title) {
	auto width = 0.0;
	auto height = 0.0;
	emscripten_get_element_css_size("#canvas", &width, &height);
	return SDL_CreateWindow(
		title.data(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
}

SDL_GLContext EngineImpl::createContext(SDL_Window* window) {
	auto context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SetSwapInterval(1);
	return context;
}

void EngineImpl::run(const UpdateCallback& updateCallback, const ShutdownCallback& shutdownCallback) {
	_updateCallback = updateCallback;
	_shutdownCallback = shutdownCallback;
	emscripten_set_main_loop(_staticUpdateCallback, 0, 1);
}

static void EngineImpl::_staticUpdateCallback() {
	if (!_updateCallback()) {
		emscripten_cancel_main_loop();
		_shutdownCallback();
	}
}

#else

void EngineImpl::setupGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

SDL_Window* EngineImpl::createWindow(std::string_view title) {
	return SDL_CreateWindow(
		title.data(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
}

SDL_GLContext EngineImpl::createContext(SDL_Window* window) {
	auto context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SetSwapInterval(1);
	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	if (version == 0) {
		// print errors
	}
	return context;
}

void EngineImpl::run(const UpdateCallback& updateCallback, const ShutdownCallback& shutdownCallback) {
	while (updateCallback()) {}
	shutdownCallback();
}

#endif
