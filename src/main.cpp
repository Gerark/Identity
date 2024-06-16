#include "LittleEngine.h"

#include "imgui/imgui.h"

bool showWindow = true;
extern "C" int main(int /*argc*/, char** /*argv*/) {
	LittleEngine engine;
	engine.init();

	engine.run([] () {
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File")) {
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

		ImGui::EndMainMenuBar();
		return true;
	});

	return 0;
}
