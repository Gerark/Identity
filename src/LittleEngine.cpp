#include "LittleEngine.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void LittleEngine::init() {
	_initSDL();
	_engine.setupGLAttributes();
	_window = _engine.createWindow("Antonino Liconti - CV");
	_context = _engine.createContext(_window);
	_initImGui();
}

void LittleEngine::run(const std::function<void()>& updateCallback) {
	_updateCallback = updateCallback;
	_engine.run([this] () {
		_internalLoop();
	});
}

void LittleEngine::_initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		printf("Error: %s\n", SDL_GetError());
	}
	printf("TEST\n");
}

void LittleEngine::_initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(_window, _context);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();

	//ImGuiIO& io = ImGui::GetIO();

	// Load Fonts
	// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
	// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
	// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
	// io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
	// io.Fonts->AddFontDefault();
}

void LittleEngine::_internalLoop() {
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

	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(_window);
}
