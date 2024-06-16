#include "LittleEngine.h"

#include "imgui/imgui.h"

bool showWindow = true;
extern "C" int main(int /*argc*/, char** /*argv*/) {
	LittleEngine engine;
	engine.init();

	engine.run([] () {
		ImGui::Begin("Tao Mei!", &showWindow);
		ImGui::Text("Test Test Test!");
		if (ImGui::Button("Close Me")) {
			showWindow = false;
		}
		ImGui::End();
		return true;
	});

	return 0;
}
