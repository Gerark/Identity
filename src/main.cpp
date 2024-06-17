#include "LittleEngine.h"

#include "imgui/imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool showWindow = true;
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

	engine.run([texture] () {
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("About Me")) {
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				// Open
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				// Save
			}
			if (ImGui::MenuItem("Close", "Ctrl+W")) {
				// Close
			}
			ImGui::EndMenu();
		}

		ImGui::Begin("Hello!");
		ImGui::Image((void*)(intptr_t)texture, ImVec2(64, 64));
		ImGui::End();

		ImGui::EndMainMenuBar();
		return true;
	});

	return 0;
}
