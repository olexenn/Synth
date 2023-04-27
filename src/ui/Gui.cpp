//
//  Gui.cpp
//  synth
//
//  Created by Алексей Дудник on 20.12.2022.
//
#include <glad/glad.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui-knobs.h>

#include <GLFW/glfw3.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES/gl2.h>
#endif

#include "ui/Gui.h"
#include "ui/IconsFontaudio.h"
#include "audio/Synth.h"

#include <iostream>

#define DEBUG 0

float val = 50.0f;
float val2 = 100.0f;
float val3 = 75.0f;
float val4 = 25.0f;
float val5 = 60.0f;
float val6 = 10.0f;

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

Gui::Gui()
{

    m_colors[WHITE] = ImColor(221, 199, 161);
    m_colors[PINK] = ImColor(211, 134, 155);
    m_colors[BLACK] = ImColor(20, 22, 23);
    m_colors[BLUE] = ImColor(125, 174, 163);
    m_colors[YELLOW] = ImColor(216, 166, 87);
    m_colors[RED] = ImColor(234, 105, 98);
    m_colors[GREEN] = ImColor(169, 182, 101);
    m_colors[AQUA] = ImColor(137, 180, 120);
    m_colors[ORANGE] = ImColor(231, 138, 78);
    m_colors[BROWN] = ImColor(101, 71, 53);
    m_colors[UNKNOWN_YELLOW] = ImColor(234, 162, 39);
    m_keyboard = new Keyboard(m_colors);
    
    float highDPIScaleFactor = 1.1;
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        exit(1);
    
    const char *glsl_version = "#version 150";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    m_window = glfwCreateWindow(1280, 720, "Synth", NULL, NULL);
    if (m_window == NULL) exit(1);
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

Gui::Gui(Audio *audio) : m_audio(audio)
{
    m_colors[WHITE] = ImColor(221, 199, 161);
    m_colors[PINK] = ImColor(211, 134, 155);
    m_colors[BLACK] = ImColor(20, 22, 23);
    m_colors[BLUE] = ImColor(125, 174, 163);
    m_colors[YELLOW] = ImColor(216, 166, 87);
    m_colors[RED] = ImColor(234, 105, 98);
    m_colors[GREEN] = ImColor(169, 182, 101);
    m_colors[AQUA] = ImColor(137, 180, 120);
    m_colors[GREY] = ImColor(80, 73, 69);
    m_colors[GREY_2] = ImColor(146, 131, 116);
    m_colors[GREY_3] = ImColor(168, 153, 132);
    m_colors[WHITE_2] = ImColor(212, 190, 152, 50);
    m_colors[WHITE_3] = ImColor(212, 190, 152);
    m_colors[ORANGE] = ImColor(231, 138, 78);
    m_colors[DARK_BLUE] = ImColor(13, 49, 56);
    m_colors[DARK_GREEN] = ImColor(50, 54, 26);
    m_colors[DARK_GREEN_1] = ImColor(51, 62, 52);
    m_colors[BROWN] = ImColor(79, 56, 41);
    m_colors[UNKNOWN_YELLOW] = ImColor(234, 162, 39);
    m_colors[PINK_BLUE] = ImColor(168, 154, 159);
    m_colors[YELLOW_RED] = ImColor(225, 136, 93);
    m_colors[RED_GREEN] = ImColor(202, 144, 100);
    m_colors[AQUA_ORANGE] = ImColor(184, 159, 99);
    m_colors[ORANGE_PINK] = ImColor(221, 136, 117);
    m_keyboard = new Keyboard(m_colors);
//    this->m_keyboard = Keyboard(m_colors);
    // initialize ImGUI with GLFW
    float highDPIScaleFactor = 1.1;
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        exit(1);
    
    const char *glsl_version = "#version 150";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    m_window = glfwCreateWindow(1280, 720, "Synth", NULL, NULL);
    if (m_window == NULL) exit(1);
    glfwSetWindowSizeLimits(m_window, 1080, 720, 2560, 1600);
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
    ImFontConfig cfg;
    cfg.OversampleH = cfg.OversampleV = 3;

    std::string mainFontFileName = Synth::getCurrentPath() + "/fonts/Montserrat-Regular.ttf";
    m_mainFont = io.Fonts->AddFontFromFileTTF(mainFontFileName.c_str(), 13.0f, &cfg);
    m_headerFont = io.Fonts->AddFontFromFileTTF(mainFontFileName.c_str(), 20.0f, &cfg);

    std::cout << "Main font " << m_mainFont->IsLoaded() << std::endl;
    std::cout << "Header font " << m_headerFont->IsLoaded() << std::endl;
    
    float iconFontSize = 25.0f * 2.0f / 3.0f;
//    float iconFontSize = 20.0f;
    static const ImWchar icons_range[] = { ICON_MIN_FAD, ICON_MAX_16_FAD, 0 };
    ImFontConfig iconCfg;
    iconCfg.MergeMode = true;
    iconCfg.PixelSnapH = true;
    iconCfg.GlyphMinAdvanceX = iconFontSize;
    std::string iconFontFileName = Synth::getCurrentPath() + "/fonts/fontaudio.ttf";
    m_iconFont = io.Fonts->AddFontFromFileTTF(iconFontFileName.c_str(), iconFontSize, &iconCfg, icons_range);
    std::cout << "Icon font " << m_iconFont->IsLoaded() << std::endl;
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

void Gui::ui_init()
{
    auto& style = ImGui::GetStyle();
    auto Colors = style.Colors;
//    Colors[ImGuiCol_WindowBg] = ImVec4(0.078f, 0.086f, 0.090f, 1.0f);
    Colors[ImGuiCol_WindowBg] = ImColor(29, 32, 33);
    Colors[ImGuiCol_TitleBg] = ImVec4(0.157f, 0.157f, 0.157f, 1.0f);
    Colors[ImGuiCol_TitleBgActive] = ImVec4(0.157f, 0.157f, 0.157f, 1.0f);
//    Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
//    style.WindowBorderSize = 0;
//    style.WindowRounding = 0.0;
//    style.WindowPadding.x = 0;
//    style.WindowPadding.y = 0;
//    m_windowSize = ImGui::GetWindowSize();
//    m_cursorPos = ImGui::GetCursorPos();
//    m_cursorPos = ImGui::GetCursorScreenPos();
}


void Gui::checkbox(const char *id, bool *value, SynthColors color)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    float size = 20.0f;
    
    ImGui::InvisibleButton(id, ImVec2(size, size));
    auto gid = ImGui::GetID(id);
    ImGui::PushID(gid);
    
    float thickness = 1.0f;
    
    bool isHovered = ImGui::IsItemHovered();
    
    if (ImGui::IsItemHovered()) thickness = 2.0f;
    
    if (!(*value)) color = GREY;
    
    draw_list->AddRectFilled(ImVec2(pos.x + 1.0f, pos.y + 1.0f), ImVec2(pos.x + size - 1.0f, pos.y + size - 1.0f), m_colors[color], 2.0f);
    
    if (isHovered)
        draw_list->AddRectFilled(ImVec2(pos.x - 1.0f, pos.y - 1.0f), ImVec2(pos.x + size + 1.0f, pos.y + size + 1.0f), m_colors[WHITE_2], 1.0f);
    
    if (isHovered && ImGui::IsMouseClicked(0)) *value = !*value;
    
    ImGui::PopID();
}

bool Gui::waveButton(const char *label, SynthColors color, bool isSelected, bool *isEnabled = nullptr)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    if (!isSelected) color = WHITE;
    
    bool isHovered = false;
    bool isClicked = false;
    
    float size = ImGui::GetTextLineHeight();
    
    ImGui::InvisibleButton(label, ImVec2(3.0f * size, 3.0f * size));
    auto gid = ImGui::GetID(label);
    
    ImRect bb(pos, ImVec2(pos.x + 3 * size, pos.y + 3 * size));
    ImGui::ButtonBehavior(bb, gid, &isHovered, &isClicked);
    
    if (!isEnabled) color = color;
    else if (!(*isEnabled)) color = GREY;
    
    auto textSize = ImGui::CalcTextSize(label);
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + 3 * size, pos.y + 3 * size), m_colors[BLACK], 2.0f);
    draw_list->AddRect(pos, ImVec2(pos.x + 3 * size, pos.y + 3 * size), m_colors[color], 2.0f);
    ImGui::PushFont(m_iconFont);
    draw_list->AddText(ImVec2(pos.x + (3.0f * size - textSize.x) * 0.4f, pos.y + (3.0f * size - textSize.y) * 0.4f), m_colors[color], label);
    ImGui::PopFont();
    if (isHovered)
        draw_list->AddRectFilled(pos, ImVec2(pos.x + 3 * size, pos.y + 3 * size), m_colors[WHITE_2], 1.0f);
    
    if (isHovered && isClicked && *isEnabled) {
        return true;
    }
    
    return false;
    
}

void Gui::curveEditor(const char *label, SynthColors color)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    float size = ImGui::GetTextLineHeight() * 2;
    
    ImGui::InvisibleButton(label, ImVec2(9 * size, 3 * size));
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + 9 * size, pos.y + 3 * size), m_colors[BLACK]);
}

void Gui::testDraw()
{
    ui_init();
    ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoCollapse);
    
//    knob("Mnenie Vlada", &val, 0.0f, 100.0f, 1.0f, RED);
    
    ImGui::End();
}

void Gui::testRun()
{
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        testDraw();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_window);
    }
}

void Gui::noteOff(int key)
{
    m_audio->noteOff(key);
    m_keyboard->keyUp(key);
}

void Gui::noteOn(int key, int velocity)
{
    m_audio->noteOn(key);
    m_keyboard->keyDown(key, velocity);
}

void Gui::run()
{
    ui_init();
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
    m_keyboard->draw();
//    m_audio->draw(m_style);
    
    ImGui::ShowDemoWindow();
    //ImGui::ShowStyleEditor();
    
    // Master
    ImGui::Begin("Master");
    knob<float>("Panning", &Audio::m_panningValue, 0.0f, 2.0f, 0.1f, GREEN);
    ImGui::SameLine();
    knob<float>("Volume", &Audio::m_gain, 0.0f, 2.0f, 0.05f, GREEN);
    ImGui::End();
    
    // ADSR
    ImGui::Begin("ADSR", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::PushFont(m_headerFont);
    ImGui::TextColored(m_colors[YELLOW], ICON_FAD_ADSR);
    ImGui::SameLine();
    ImGui::Text("ADSR");
    ImGui::PopFont();
    ImGui::Separator();
    curveEditor("ADSR", YELLOW);
    knob<float>("Attack", &Synth::m_adsrParams.attackTime, 0.001f, 1.0f, 0.01f, YELLOW);
    ImGui::SameLine();
    knob<float>("Decay", &Synth::m_adsrParams.decayTime, 0.0f, 1.0f, 0.05f, YELLOW);
    ImGui::SameLine();
    knob<float>("Sustain", &Synth::m_adsrParams.sustainAmplitude, 0.0f, 1.0f, 0.05f, YELLOW);
    ImGui::SameLine();
    knob<float>("Release", &Synth::m_adsrParams.releaseTime, 0.01f, 1.0f, 0.01f, YELLOW);
    ImGui::End();
    
    // OSCs
    for (int i = 0; i < 3; i++) {
        std::string name = "OSC  " + std::to_string(i + 1);
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);
        checkbox(name.c_str(), &Synth::m_isOscActive[i], PINK);
        ImGui::SameLine();
        ImGui::PushFont(m_headerFont);
//        ImGui::Text("%s", name.c_str());
        ImGui::TextColored(Synth::m_isOscActive[i] ? m_colors[WHITE] : m_colors[GREY], "%s", name.c_str());
        ImGui::PopFont();
        ImGui::Separator();
        knob<float>("Gain", &Synth::m_oscGain[i], 0.0f, 1.0f, 0.05f, PINK, &Synth::m_isOscActive[i]);
        ImGui::SameLine();
        knob<int>("Offset", &Synth::m_oscNoteOffset[i], 0, 48, 12, PINK, &Synth::m_isOscActive[i]);
//        ImGui::SameLine();
        
//        waveSelect(&Synth::m_oscType[i], PINK);
        if (waveButton(ICON_FAD_MODSINE, PINK, Synth::m_oscType[i] == SINE, &Synth::m_isOscActive[i])) Synth::m_oscType[i] = SINE;
        ImGui::SameLine();
        if (waveButton(ICON_FAD_MODSQUARE, PINK, Synth::m_oscType[i] == SQUARE, &Synth::m_isOscActive[i])) Synth::m_oscType[i] = SQUARE;
        ImGui::SameLine();
        if (waveButton(ICON_FAD_MODSAWUP, PINK, Synth::m_oscType[i] == SAW, &Synth::m_isOscActive[i])) Synth::m_oscType[i] = SAW;
        ImGui::SameLine();
        if (waveButton(ICON_FAD_MODTRI, PINK, Synth::m_oscType[i] == TRIANGLE, &Synth::m_isOscActive[i])) Synth::m_oscType[i] = TRIANGLE;
        ImGui::SameLine();
        if (waveButton(ICON_FAD_MODRANDOM, PINK, Synth::m_oscType[i] == NOISE, &Synth::m_isOscActive[i])) Synth::m_oscType[i] = NOISE;
        ImGui::End();
    }
    
    // FILTER
    ImGui::Begin("Filter", nullptr, ImGuiWindowFlags_NoTitleBar);
    checkbox("Active", &Synth::m_filter.getActivity(), AQUA);
    ImGui::SameLine();
    ImGui::PushFont(m_headerFont);
    ImGui::Text("FILTER");
    ImGui::PopFont();
    ImGui::Separator();
    if (ImGui::Selectable("LowPass", Synth::m_filter.getFilterType() == LOW_PASS))
        Synth::m_filter.setFilterType(LOW_PASS);
    if (ImGui::Selectable("HighPass", Synth::m_filter.getFilterType() == HIGH_PASS))
        Synth::m_filter.setFilterType(HIGH_PASS);
    knob<float>("Low Pass", &Synth::m_filter.getLowCuttoff(), 100.0f, 20000.0f, 200.0f, AQUA, &Synth::m_filter.getActivity());
    ImGui::SameLine();
    knob<float>("High Pass", &Synth::m_filter.getHighCuttoff(), 100.0f, 3000.0f, 20.0f, AQUA, &Synth::m_filter.getActivity());
    ImGui::End();
    
    // LFO
    ImGui::Begin("LFO", nullptr, ImGuiWindowFlags_NoTitleBar);
//    ImGui::Checkbox("Active", &Synth::m_isLfoActive);
    checkbox("LFO", &Synth::m_isLfoActive, RED);
    ImGui::SameLine();
    ImGui::PushFont(m_headerFont);
    ImGui::Text("LFO");
    ImGui::PopFont();
    ImGui::Separator();
    knob<float>("Freq", &Synth::m_lfoFrequency, 1.0f, 100.0f, 1.0f, RED, &Synth::m_isLfoActive);
    ImGui::End();
    
    // PRESET MANAGER
    ImGui::Begin("Presets");
    static const char *currentItem = Synth::m_presets[0].c_str();
    int numberOfPresets = static_cast<int>(Synth::m_presets.size());
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
    
//    ImGui::Begin("Meter");
//    ImGui::Text("Meter");
//    ImGui::End();
//
//    ImGui::Begin("Visual");
//    ImGui::Text("Visual");
//    ImGui::End();
}

