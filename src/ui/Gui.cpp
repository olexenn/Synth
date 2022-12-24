//
//  Gui.cpp
//  synth
//
//  Created by Алексей Дудник on 20.12.2022.
//
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui-knobs.h>

#include <GLFW/glfw3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES/gl2.h>
#endif

#include "ui/Gui.h"
#include "audio/Synth.h"

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
}

Gui::Gui(Audio *audio) : m_audio(audio)
{
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
    
    m_window = glfwCreateWindow(1280, 720, "Synth", NULL, NULL);
    if (m_window == NULL) return 1;
    glfwSetWindowSizeLimits(m_window, 1080, 720, 1280, 720);
    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
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
    
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_audio->stop();
    m_audio->close();
}

void Gui::noteOff(int key)
{
    m_audio->noteOff(key);
    m_keyboard.keyUp(key);
}

void Gui::noteOn(int key, int velocity)
{
    m_audio->noteOn(key);
    m_keyboard.keyDown(key, velocity);
}

void Gui::run()
{
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        for (const auto &keyCode : keyCodeMap) {
            if (ImGui::IsKeyDown(keyCode.first))
                noteOn(keyCode.second, 1);
            else if (ImGui::IsKeyReleased(keyCode.first))
                noteOff(keyCode.second);
        }
        
        draw();
        
#if DEBUG
        ImGui::Begin("Debug");

        auto currentNotes = m_keyboard.getKeys();
        

        ImGui::Text("Playing notes: ");

        for (const auto &note : currentNotes) {
            ImGui::SameLine();
            ImGui::Text("%d", note);
        }
        
        auto voices = m_audio->getVoices();
        for (const auto &voice : voices) {
            ImGui::Text("Voice %d\r status: %d", voice->getKey(), voice->isActive());
        }

        ImGui::Text("Time: %f", m_audio->getTime());
        
        float sample = m_audio->getSample();
        if (sample > 1.0f || sample < -1.0f)
            ImGui::Text("Sample peaked");
        else
            ImGui::Text("Sample ok");
        ImGui::Text("Sample: %f", m_audio->getSample());
        

        ImGui::Text("Polyphony counter: %d", m_audio->getCounter());

        ImGui::End();
#endif
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_window);
//        ImGui::UpdatePlatformWindows();
        
    }
}

void Gui::draw()
{
    m_keyboard.draw();
//    m_audio->draw(m_style);
    
    // Master
    ImGui::Begin("Master");
    ImGuiKnobs::Knob("Panning", &Audio::m_panningValue, 0.0f, 2.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Volume", &Audio::m_gain, 0.0f, 2.0f);
    ImGui::End();
    
    // ADSR
    ImGui::Begin("ADSR");
    ImGuiKnobs::Knob("Attack", &Synth::m_adsrParams.attackTime, 0.001f, 1.0f, 0.01f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &Synth::m_adsrParams.decayTime, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &Synth::m_adsrParams.releaseTime, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &Synth::m_adsrParams.releaseTime, 0.01f, 1.0f, 0.01f);
    ImGui::End();
    
    // OSCs
    for (int i = 0; i < 3; i++) {
        std::string name = "Oscillator " + std::to_string(i + 1);
        ImGui::Begin(name.c_str());
        ImGuiKnobs::Knob("Gain", &Synth::m_oscGain[i], 0.0f, 1.0f, 0.05f);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Offset", &Synth::m_oscNoteOffset[i], 0, 48, 12);
        ImGui::SameLine();
        ImGui::Checkbox("Active", &Synth::m_isOscActive[i]);
        if (ImGui::Selectable("Sine", Synth::m_oscType[i] == SINE)) Synth::m_oscType[i] = SINE;
        if (ImGui::Selectable("Square", Synth::m_oscType[i] == SQUARE)) Synth::m_oscType[i] = SQUARE;
        if (ImGui::Selectable("Saw", Synth::m_oscType[i] == SAW)) Synth::m_oscType[i] = SAW;
        if (ImGui::Selectable("Triangle", Synth::m_oscType[i] == TRIANGLE)) Synth::m_oscType[i] = TRIANGLE;
        if (ImGui::Selectable("Noise", Synth::m_oscType[i] == NOISE)) Synth::m_oscType[i] = NOISE;
        ImGui::End();
    }
    
    // FILTER
    ImGui::Begin("Filter");
    ImGui::Checkbox("Active", &Synth::m_filter.getActivity());
    if (ImGui::Selectable("LowPass", Synth::m_filter.getFilterType() == LOW_PASS))
        Synth::m_filter.setFilterType(LOW_PASS);
    if (ImGui::Selectable("HighPass", Synth::m_filter.getFilterType() == HIGH_PASS))
        Synth::m_filter.setFilterType(HIGH_PASS);
    ImGuiKnobs::Knob("Low Pass", &Synth::m_filter.getLowCuttoff(), 100.0f, 20000.0f, 20.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("High Pass", &Synth::m_filter.getHighCuttoff(), 100.0f, 3000.0f, 20.0f);
    ImGui::End();
    
    // LFO
    ImGui::Begin("LFO");
    ImGui::Checkbox("Active", &Synth::m_isLfoActive);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Freq", &Synth::m_lfoFrequency, 1.0f, 100.0f, 1.0f);
    ImGui::End();
    
    // PRESET MANAGER
    ImGui::Begin("Presets");
    static const char *currentItem = Synth::m_presets[0].c_str();
    auto numberOfPresets = Synth::m_presets.size();
    float w = ImGui::CalcItemWidth();
    float spacing = m_style.ItemSpacing.x;
    float buttonSz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - buttonSz * 2.0f);
    if (ImGui::BeginCombo("Presets", currentItem, ImGuiComboFlags_NoArrowButton)) {
        for (int i = 0; i < numberOfPresets; i++) {
            bool isSelected = (currentItem == Synth::m_presets[i]);
            if (ImGui::Selectable(Synth::m_presets[i].c_str())) {
                currentItem = Synth::m_presets[i].c_str();
                Synth::loadPreset(currentItem);
                break;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##l", ImGuiDir_Left)) {
        if (--Synth::m_counter < 0)
            Synth::m_counter = numberOfPresets - 1;
        currentItem = Synth::m_presets[Synth::m_counter].c_str();
        Synth::loadPreset(currentItem);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
        if (++Synth::m_counter == numberOfPresets)
            Synth::m_counter = 0;
        currentItem = Synth::m_presets[Synth::m_counter].c_str();
        Synth::loadPreset(currentItem);
    }
    ImGui::SameLine(0, m_style.ItemInnerSpacing.x);
    if (ImGui::Button("Save")) ImGui::OpenPopup("Save new preset");
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal("Save new preset", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char buf[64] = "";
        ImGui::Text("Enter the name for new preset");
        ImGui::InputText("Name", buf, 64);
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (std::strcmp(buf, "") || std::strcmp(buf, " ")) ImGui::CloseCurrentPopup();
            Synth::savePreset(buf);
            Synth::getAllPresets();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    ImGui::End();
}

