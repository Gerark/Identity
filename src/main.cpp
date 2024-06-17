#include "LittleEngine.h"

#include "imgui/imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <unordered_map>
#include <memory>

struct Page {
	std::function<void()> renderFunction;
};

class App {
public:
	void render(GLuint texture) {
		auto& style = ImGui::GetStyle();

		ImGui::BeginMainMenuBar();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x + 8, style.FramePadding.y));
		if (ImGui::Button("Home")) {
			test = true;
		}
		if (ImGui::Button("Portfolio")) {
		}
		if (ImGui::Button("Resume")) {
		}
		if (ImGui::Button("About Me")) {
		}
		ImGui::PopStyleVar();

		ImGui::EndMainMenuBar();

		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image((void*)(intptr_t)texture, ImVec2(64, 64));
		ImGui::End();
	}

private:
	bool test = false;
	std::unordered_map<std::string, std::unique_ptr<Page>> _pages;
};

extern "C" int main(int /*argc*/, char** /*argv*/) {
	LittleEngine engine;
	engine.init();

	GLuint texture;

	int width;
	int height;
	int channelsCount;
	unsigned char* textureData = stbi_load("data/char.png", &width, &height, &channelsCount, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, channelsCount > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	App app;
	engine.run([texture, &app] () {
		app.render(texture);
		return true;
	});

	return 0;
}
