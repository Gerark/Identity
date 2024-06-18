#include "LittleEngine.h"

#include "imgui/imgui.h"

#include <unordered_map>
#include <vector>
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
	App(LittleEngine& engine): _engine(engine) {
		_oneDoesNotTexture = _engine.loadTexture("data/char.png");
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
		auto displaySize = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({displaySize.x - style.ItemSpacing.x * 2, displaySize.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
		ImGui::Dummy({0, 32});
		_pushFontSize(32);
		_centeredText("Welcome to my website!");
		ImGui::Dummy({0, 16});
		_popFontSize();
		/*
		_pushFontSize(24);
		_centeredText("I'm Antonino Liconti ( Gerark )");
		_centeredText("A Senior Software Developer passionate about the gaming industry, UI/UX, and narrative storytelling.");
		_popFontSize();
		*/

		auto textureSize = _oneDoesNotTexture.toImGuiSize();
		auto ratio = textureSize.y / textureSize.x;
		textureSize.y = 0.1f * displaySize.y;
		textureSize.x = textureSize.y / ratio;
		ImGui::Image(_oneDoesNotTexture.toImGui(), textureSize);

		//ImGui::Dummy({0, 100});
		_pushFontSize(18);
		_centeredText("This is a WebAssembly OpenGL application written in C++, utilizing ImGui to display the user interface.");
		_popFontSize();

		ImGui::PopStyleColor();

		ImGui::Dummy({0, 32});
		ImVec2 windowCenter = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetCursorPosY());
		// Set the cursor to the calculated center position
		ImGui::SetCursorPosX(windowCenter.x - ImGui::CalcTextSize("Check the GitHub repo").x * 0.5f);

		bool checkRepoPressed = ImGui::Button("Check the GitHub repo");
		if (checkRepoPressed) {
			_engine.openUrl("https://github.com/Gerark/Identity");
		}

		ImGui::End();
	}

	void _renderResume() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image(_oneDoesNotTexture.toImGui(), ImVec2(64, 64));
		ImGui::End();
	}

	void _renderPortfolio() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image(_oneDoesNotTexture.toImGui(), ImVec2(64, 64));
		ImGui::End();
	}

	void _renderAboutMe() {
		auto& style = ImGui::GetStyle();
		auto size = ImGui::GetIO().DisplaySize;
		auto cursorPos = ImGui::GetCursorPos();
		ImGui::SetNextWindowSize({size.x - style.ItemSpacing.x * 2, size.y - style.ItemSpacing.y * 2 - cursorPos.y});
		ImGui::SetNextWindowPos(cursorPos);
		ImGui::Begin("Hello!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Image(_oneDoesNotTexture.toImGui(), ImVec2(64, 64));
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

	ImVec2 _centeredRect(ImVec2 size) {
		ImVec2 windowSize = ImGui::GetWindowSize();
		float windowWidth = windowSize.x;

		// Calculate the indentation to center the text
		float indent = (windowWidth - size.x) / 2.0f;

		// Ensure the indentation is not negative
		if (indent > 0.0f) {
			ImGui::Indent(indent);
		}

		return ImVec2(indent, 0);
	}

	bool test = false;
	std::unordered_map<PageType, std::unique_ptr<Page>> _pages;
	Page* _currentPage{};
	TextureInfo _oneDoesNotTexture;
	LittleEngine& _engine;
};

extern "C" int main(int /*argc*/, char** /*argv*/) {
	LittleEngine engine;
	engine.init();

	App app(engine);
	engine.run([&app] () {
		app.render();
		return true;
	});

	return 0;
}
