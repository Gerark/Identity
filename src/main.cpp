#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
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

SDL_Window *g_window;
SDL_GLContext context;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool show_another_window = false;
int g_width;
int g_height;

// Function used by c++ to get the size of the html canvas
/*EM_JS(int, canvas_get_width, (), {
    return Module.canvas.width;
    });
    */
    // Function used by c++ to get the size of the html canvas
    /*EM_JS(int, canvas_get_height, (), {
        return Module.canvas.height;
        });
        */
        // Function called by javascript
        /*EM_JS(void, resizeCanvas, (), {
            js_resizeCanvas();
        });*/
int canvas_get_width() {
    return 800;
}

int canvas_get_height() {
    return 600;
}

void on_size_changed() {
    // glfwSetWindowSize(g_window, g_width, g_height);

    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void loop() {
    int width = canvas_get_width();
    int height = canvas_get_height();

    if (width != g_width || height != g_height) {
        g_width = width;
        g_height = height;
        on_size_changed();
    }

    // glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                            // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window) {
        // ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        // ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Render();

    //int display_w, display_h;
    SDL_GL_MakeCurrent(g_window, context);
    // glfwMakeContextCurrent(g_window);
    // glfwGetFramebufferSize(g_window, &display_w, &display_h);
    // glViewport(0, 0, 1000, 1000);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Error: " << error << std::endl;
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glfwMakeContextCurrent(g_window);
    SDL_GL_SwapWindow(g_window);
}

int init_gl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Open a window and create its OpenGL context
    int canvasWidth = g_width;
    int canvasHeight = g_height;
    g_window =
        SDL_CreateWindow("Hello Triangle Minimal",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            canvasWidth, canvasHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    context = SDL_GL_CreateContext(g_window);
    if (g_window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        //   glfwTerminate();
        return -1;
    }
    SDL_GL_MakeCurrent(g_window, context);
    // glfwMakeContextCurrent(g_window); // Initialize GLEW

#ifndef __EMSCRIPTEN__
    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (version == 0) {
        // print errors
    }
#endif

    return 0;
}

int init_imgui() {
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(g_window, context);
    ImGui_ImplOpenGL3_Init();

    // Setup style
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();

    // Load Fonts
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
    // io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
    // io.Fonts->AddFontDefault();

    // resizeCanvas();

    return 0;
}

int init() {
    init_gl();
    init_imgui();
    return 0;
}

void quit() {
    // glfwTerminate();
}

extern "C" int main(int argc, char **argv) {
    g_width = canvas_get_width();
    g_height = canvas_get_height();
    if (init() != 0)
        return 1;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (true) {
        loop();
    }
#endif

    quit();

    return 0;
}
