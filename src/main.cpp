#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui-knobs.h>

#include <GLFW/glfw3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

//#include <iostream>
#include <portaudio.h>
#include <cmath>
#include <array>
#include <map>

#include "Keyboard.h"
#include "Audio.h"

#define DEBUG 1

std::map<ImGuiKey, int> keyCodeMap {
    { ImGuiKey_Z, 39 },
    { ImGuiKey_S, 40 },
    { ImGuiKey_X, 41 },
    { ImGuiKey_D, 42 },
    { ImGuiKey_C, 43 },
    { ImGuiKey_V, 44 },
    { ImGuiKey_G, 45 },
    { ImGuiKey_B, 46 },
    { ImGuiKey_H, 47 },
    { ImGuiKey_N, 48 },
    { ImGuiKey_J, 49 },
    { ImGuiKey_M, 50 },
    { ImGuiKey_Q, 51 },
    { ImGuiKey_2, 52 },
    { ImGuiKey_W, 53 },
    { ImGuiKey_3, 54 },
    { ImGuiKey_E, 55 },
    { ImGuiKey_R, 56 },
    { ImGuiKey_5, 57 },
    { ImGuiKey_T, 58 },
    { ImGuiKey_6, 59 },
    { ImGuiKey_Y, 60 },
    { ImGuiKey_7, 61 },
    { ImGuiKey_U, 62 },
    { ImGuiKey_I, 63 },
    { ImGuiKey_9, 64 },
    { ImGuiKey_O, 65 },
    { ImGuiKey_0, 66 },
    { ImGuiKey_P, 67 }
};

static void glfwErrorCallback(int error, const char *description)
{
    exit(1);
//    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(void) {
    // initialize ImGUI with GLFW
    float highDPIScaleFactor = 1.1;
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        return 1;
    
    const char *glsl_version = "#version 150";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Synth", NULL, NULL);
    if (window == NULL) return 1;
    glfwSetWindowSizeLimits(window, 1080, 720, 1280, 720);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // killed my 5 hours
    glfwSwapInterval(1); // vsync
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    ImGui::StyleColorsClassic();
    
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    
    // Audio Init
    Audio audio;

    audio.open(Pa_GetDefaultOutputDevice());
    
    Keyboard keyboard;
    
    audio.start();
    
    std::array<int, 20> keysPressed = {0};
    
    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // key events
        for (const auto &keyCode : keyCodeMap) {
            if (ImGui::IsKeyDown(keyCode.first)) {
                audio.noteOn(keyCode.second);
                keyboard.keyDown(keyCode.second, 1);
            }
            else if (ImGui::IsKeyReleased(keyCode.first)) {
                audio.noteOff(keyCode.second);
                keyboard.keyUp(keyCode.second);
            }
        }
        
        // keyboard ui
        keyboard.draw();
        
        // synth ui
        audio.draw(style);
        
        ImGui::ShowDemoWindow();

#if DEBUG
        
        // debug info
        ImGui::Begin("Debug");

        auto currentNotes = keyboard.getKeys();
        

        ImGui::Text("Playing notes: ");

        for (const auto &note : currentNotes) {
            ImGui::SameLine();
            ImGui::Text("%d", note);
        }
        
        auto voices = audio.getVoices();
        for (const auto &voice : voices) {
            ImGui::Text("Voice %d\r status: %d", voice->getKey(), voice->isActive());
        }

        ImGui::Text("Time: %f", audio.getTime());
        
        float sample = audio.getSample();
        if (sample > 1.0f || sample < -1.0f)
            ImGui::Text("Sample peaked");
        else
            ImGui::Text("Sample ok");
        ImGui::Text("Sample: %f", audio.getSample());
        

        ImGui::Text("Polyphony counter: %d", audio.getCounter());

        ImGui::End();
        
#endif
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
//        ImGui::UpdatePlatformWindows();
    }
    
    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    audio.stop();
    audio.close();
    
    return 0;
}
