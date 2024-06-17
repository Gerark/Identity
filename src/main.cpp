#include "LittleEngine.h"

#include "imgui/imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <unordered_map>
#include <memory>
#include <string>

enum class PageType {
	Home,
	Portfolio,
	Resume,
	AboutMe
};

struct Page {
	Page(std::string title, std::function<void()> renderFunction): title(std::move(title)), renderFunction(std::move(renderFunction)) {}
	std::string title;
	std::function<void()> renderFunction;
};

class App {
public:
	App(GLuint texture, LittleEngine& engine): _texture(texture), _engine(engine) {
		_pages.emplace(PageType::Home, std::make_unique<Page>("Home", [this] () { _renderHome(); }));
		_pages.emplace(PageType::Portfolio, std::make_unique<Page>("Portfolio", [this] () { _renderPortfolio(); }));
		_pages.emplace(PageType::Resume, std::make_unique<Page>("Resume", [this] () { _renderResume(); }));
		_pages.emplace(PageType::AboutMe, std::make_unique<Page>("About Me", [this] () { _renderAboutMe(); }));
		_selectPage(PageType::Home);
	}

	void render() {
		auto& style = ImGui::GetStyle();

		ImGui::BeginMainMenuBar();

		for (auto&& [pageType, page] : _pages) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x + 8, style.FramePadding.y));
			ImGuiCol_ styleToUse = page.get() == _currentPage ? ImGuiCol_ButtonActive : ImGuiCol_Button;
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[styleToUse]);
			bool isPressed = ImGui::Button(page->title.c_str());
			if (isPressed) {
				_selectPage(pageType);
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		ImGui::EndMainMenuBar();

		if (_currentPage) {
			_currentPage->renderFunction();
		}
	}

private:

	void _renderHome() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image((void*)(intptr_t)_texture, ImVec2(64, 64));

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
		_pushFontSize(32);
		_centeredText("Welcome to my website!");
		ImGui::Dummy({0, 16});
		_popFontSize();

		_pushFontSize(24);
		_centeredText("I'm \"Gerark\" Antonino Liconti");
		_centeredText("A Senior Software Developer passionate about the gaming industry, UI/UX, and narrative storytelling.");
		_popFontSize();
		ImGui::PopStyleColor();

		ImGui::End();
	}

	void _renderResume() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image((void*)(intptr_t)_texture, ImVec2(64, 64));
		ImGui::End();
	}

	void _renderPortfolio() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image((void*)(intptr_t)_texture, ImVec2(64, 64));
		ImGui::End();
	}

	void _renderAboutMe() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image((void*)(intptr_t)_texture, ImVec2(64, 64));
		ImGui::End();
	}

	void _pushFontSize(unsigned int size) {
		ImGui::PushFont(_engine.getFontBySize(size));
	}

	void _popFontSize() {
		ImGui::PopFont();
	}

	void _selectPage(PageType page) {
		if (auto itr = _pages.find(page); itr != _pages.end()) {
			_currentPage = itr->second.get();
		}
	}

	void _centeredText(const char* text) {
		ImVec2 windowSize = ImGui::GetWindowSize();
		float windowWidth = windowSize.x;

		// Measure the text width
		ImVec2 textSize = ImGui::CalcTextSize(text, nullptr, true, windowWidth);

		// Calculate the indentation to center the text
		float indent = (windowWidth - textSize.x) / 2.0f;

		// Ensure the indentation is not negative
		if (indent > 0.0f) {
			ImGui::Indent(indent);
		}

		ImGui::TextWrapped("%s", text);

		if (indent > 0.0f) {
			ImGui::Unindent(indent);
		}
	}

	bool test = false;
	std::unordered_map<PageType, std::unique_ptr<Page>> _pages;
	Page* _currentPage{};
	GLuint _texture;
	LittleEngine& _engine;
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

	App app(texture, engine);
	engine.run([texture, &app] () {
		app.render();
		return true;
	});

	return 0;
}
