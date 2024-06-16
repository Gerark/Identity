#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL_opengles2.h>
#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#else _WIN32
#include <SDL.h>
#include <glad/gl.h>
#endif


#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <iostream>
#include <functional>

#ifdef __EMSCRIPTEN__

class EngineImpl {
public:
	void setupGLAttributes() {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	}

	SDL_Window* createWindow() {
		auto width = 0.0;
		auto height = 0.0;
		emscripten_get_element_css_size("#canvas", &width, &height);
		return SDL_CreateWindow("Antonino Liconti - CV",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	}

	SDL_GLContext createContext(SDL_Window* window) {
		auto context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, context);
		SDL_GL_SetSwapInterval(1);
		return context;
	}

	void run(const std::function<void()>& updateCallback) {
		_updateCallback = updateCallback;
		emscripten_set_main_loop(_staticUpdateCallback, 0, 1);
	}

private:

	static void _staticUpdateCallback() {
		_updateCallback();
	}

	static std::function<void()> _updateCallback;
};

std::function<void()> EngineImpl::_updateCallback;

#else

class EngineImpl {
public:
	void setupGLAttributes() {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	}

	SDL_Window* createWindow() {
		return SDL_CreateWindow("Antonino Liconti - CV",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1280,
			720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	}

	SDL_GLContext createContext(SDL_Window* window) {
		auto context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, context);
		SDL_GL_SetSwapInterval(1);
		int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
		if (version == 0) {
			// print errors
		}
		return context;
	}

	void run(const std::function<void()>& updateCallback) {
		while (true) {
			updateCallback();
		}
	}
};

#endif

class LittleEngine {
public:
	void init() {
		_initSDL();
		_engine.setupGLAttributes();
		_window = _engine.createWindow();
		_context = _engine.createContext(_window);
		_initImGui();
	}

	void run(const std::function<void()>& updateCallback) {
		_updateCallback = updateCallback;
		_engine.run([this] () {
			_internalLoop();
		});
	}

private:
	void _initSDL() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
			printf("Error: %s\n", SDL_GetError());
		}
		printf("TEST\n");
	}

	void _initImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplSDL2_InitForOpenGL(_window, _context);
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		// Load Fonts
		// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
		// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
		// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
		// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
		// io.Fonts->AddFontDefault();
	}

	void _internalLoop() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				SDL_Quit();
				exit(0);
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		_updateCallback();

		ImGui::Render();

		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		auto error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cout << "Error: " << error << std::endl;
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(_window);
	}

	EngineImpl _engine;
	SDL_Window* _window;
	SDL_GLContext _context;
	std::function<void()> _updateCallback;
	ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

bool showWindow = true;
extern "C" int main(int argc, char** argv) {
	LittleEngine engine;
	engine.init();

	engine.run([] () {
		ImGui::Begin("Tao Mei!", &showWindow);
		ImGui::Text("Test Test Test!");
		if (ImGui::Button("Close Me")) {
			showWindow = false;
		}
		ImGui::End();
	});

	return 0;
}
