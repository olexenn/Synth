//
//  Gui.h
//  synth
//
//  Created by Алексей Дудник on 20.12.2022.
//

#ifndef Gui_h
#define Gui_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <map>

#include <imgui.h>

#include "ui/Keyboard.h"
#include "audio/Audio.h"

//enum SynthColors
//{
//    WHITE,
//    WHITE_2,
//    PINK,
//    BLUE,
//    YELLOW,
//    RED,
//    GREEN,
//    AQUA,
//    BLACK,
//    GREY,
//    NUMBER_OF_COLORS
//};

class Gui
{
public:
    Gui(Audio *audio);
    Gui();
    ~Gui();
    void testRun();
    void run();
    
private:
    GLFWwindow *m_window;
    ImGuiStyle m_style;
    Keyboard *m_keyboard;
    Audio *m_audio;
    ImVec2 m_windowSize;
    ImVec2 m_cursorPos;
//    ImColor m_colors[NUMBER_OF_COLORS];
    std::array<ImColor, NUMBER_OF_COLORS> m_colors;
    ImFont *m_mainFont;
    ImFont *m_headerFont;
    ImFont *m_iconFont;
    
    void noteOff(int key);
    void noteOn(int key, int velocity);
    void draw();
    void ui_init();
    void testDraw();
    
    template<typename T>
    void knob(const char *label, T *value, T min, T max, T step, SynthColors color, bool *isEnabled = nullptr)
    {
        if (*value > max) *value = max;
        else if (*value < min) *value = min;

        float size = ImGui::GetTextLineHeight() * 2.0f;

        auto io = ImGui::GetIO();

        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        float outer_radius = size * 0.83f;
        float inner_radius = size * 0.8f;

        float angleMax = M_PI * 2.25f; // 0.25f * M_PI
        float angleMin = M_PI * 0.75f; // -1.25 * M_PI
        float angleRange = angleMax - angleMin;
        float valueRange = max - min;
        float angle = angleMin + angleRange * ((*value - min) / valueRange);
//        ImVec2 tickCenter = ImVec2(cursorPos.x + outer_radius - 1.0f, cursorPos.y + outer_radius - 1.0f);
        float centerX = cursorPos.x + 2.2f * size / 2;
        ImVec2 tickCenter = ImVec2(centerX - 0.5f, cursorPos.y + outer_radius);
        ImVec2 tickPos = ImVec2(tickCenter.x + cosf(angle) * outer_radius, tickCenter.y + sinf(angle) * outer_radius);

        ImGui::InvisibleButton(label, ImVec2(2.2f * size, 2.2f * size));
        

        auto gid = ImGui::GetID(label);
        ImGui::PushID(gid);

    //    ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        bool isActive = ImGui::IsItemActive();
        bool isHovered = ImGui::IsItemHovered();

        float tickThickness = 3.0f;
        float arcThickness = 2.5f;

        if (isActive || isHovered) {
            tickThickness = 4.0f;
            arcThickness = 3.5f;
        }

        if (isActive && io.MouseDelta.x != 0.0f) {
//            *value += io.MouseDelta.x * ((max - min) / 150.0f);
//            *value = io.MouseDelta.x / ((max - min) / step);
//            *value += io.MouseDelta.x * (min + (max - min) * step);
    //        *value -= io.MouseDelta.y * ((max - min) / 500.0f);
//            *value -= io.MouseDelta.y * ((max - min) / 150.0f);
            *value -= io.MouseDelta.y * (min + (max - min) * step);
            if (*value < min) *value = min;
            if (*value > max) *value = max;
        }
        
        SynthColors textColor = WHITE;
        if (!isEnabled) color = color;
        else if (!(*isEnabled)) {
            color = GREY;
            textColor = GREY;
        }
        
        auto textWidth = ImGui::CalcTextSize(label).x;

//        draw_list->AddRectFilled(cursorPos, ImVec2(cursorPos.x + 2.2f * size, cursorPos.y + 2.2f * size), m_colors[GREY]);
        draw_list->AddCircleFilled(ImVec2(centerX , cursorPos.y + outer_radius + 1.0f), inner_radius, m_colors[BLACK]);
        draw_list->PathArcTo(ImVec2(centerX, cursorPos.y + outer_radius + 2.0f), outer_radius, angleMin, angle);
        draw_list->PathStroke(m_colors[color], false, arcThickness);
        draw_list->AddLine(tickCenter, tickPos, m_colors[textColor], tickThickness);
        draw_list->AddText(ImVec2(cursorPos.x + (2.2f * size - textWidth) * 0.5f, cursorPos.y + outer_radius + m_style.WindowPadding.y + ImGui::GetTextLineHeight()), m_colors[textColor], label);

        ImGui::PopID();
    }
    
    void checkbox(const char *id, bool *value, SynthColors color);
    bool waveButton(const char *label, SynthColors color, bool isSelected, bool *isEnabled);
    void curveEditor(const char *label, SynthColors color);
};

#endif /* Gui_h */
