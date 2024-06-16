#include "LittleEngine.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void LittleEngine::init() {
	_initSDL();
	_engine.setupGLAttributes();
	_window = _engine.createWindow("Antonino Liconti");
	_context = _engine.createContext(_window);
	_initImGui();
}

void LittleEngine::run(const UpdateCallback& updateCallback) {
	_updateCallback = updateCallback;
	_engine.run(
		[this] () { return _internalLoop(); },
		[this] () { _cleanup(); }
	);
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

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("data/mangold.ttf", 23.0f);
	io.Fonts->AddFontFromFileTTF("data/mangold.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("data/mangold.ttf", 26.0f);
	io.Fonts->AddFontFromFileTTF("data/mangold.ttf", 32.0f);
	io.Fonts->AddFontDefault();
}

bool LittleEngine::_internalLoop() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			return false;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window)) {
				return false;
			}

			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				printf("Window resized to %d,%d\n", event.window.data1, event.window.data2);
			}
			break;
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	auto exit = _updateCallback();

	ImGui::Render();

	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(_window);

	return exit;
}

void LittleEngine::_cleanup() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}
