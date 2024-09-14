//
//  Keyboard.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include <imgui.h>
#include <random>

#include "ui/Keyboard.h"

Keyboard::Keyboard(std::array<ImColor, NUMBER_OF_COLORS>& colors)
//: m_pressedColors { colors[PINK], colors[BLUE], colors[YELLOW], colors[RED], colors[GREEN], colors[AQUA] }
: m_pressedColors { colors[YELLOW], colors[BROWN] }
{
//    m_synthColors = colors;
    std::copy(std::begin(colors), std::end(colors), std::begin(m_synthColors));
//    m_pressedColors { colors[PINK], colors[BLUE], colors[YELLOW], colors[RED], colors[GREEN], colors[AQUA] };
}

void Keyboard::draw()
{
//    ImU32 Red = IM_COL32(0, 255, 0, 255);
//    ImGui::Begin("Keyboard", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Begin("Keyboard", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::Separator();
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    int width = 20;
    int curKey = 0;
    
    for (int key = 0; key < 52; key++) {
        ImU32 col = m_synthColors[WHITE];
        if (m_keyState[curKey][0]) col = m_keyState[curKey][0];
        drawList->AddRectFilled(ImVec2(pos.x + key * width, pos.y), ImVec2(pos.x + key * width + width, pos.y + 120), col, 0, ImDrawCornerFlags_All);
        drawList->AddRect(ImVec2(pos.x + key * width, pos.y), ImVec2(pos.x + key * width + width, pos.y + 120), m_synthColors[BLACK], 0, ImDrawCornerFlags_All);
        curKey++;
        if (Keyboard::hasBlack(key))
            curKey++;
    }
    
    curKey = 1;
    for (int key = 0; key < 52; key++) {
        if (Keyboard::hasBlack(key)) {
            ImU32 col = m_synthColors[BLACK];
            if (m_keyState[curKey][0]) col = m_keyState[curKey][0];
            if (m_keyState[curKey][0]) {
                drawList->AddRectFilledMultiColor(ImVec2(pos.x + key * width + width * 3 / 4, pos.y),
                                                  ImVec2(pos.x + key * width + width * 5 / 4 + 1, pos.y + 80),
                                                  m_keyState[curKey][0], m_keyState[curKey][0],
                                                  m_keyState[curKey][1], m_keyState[curKey][1]);
            }
            else {
                drawList->AddRectFilled(ImVec2(pos.x + key * width + width * 3 / 4, pos.y),
                                        ImVec2(pos.x + key * width + width * 5 / 4 + 1, pos.y + 80),
                                        col, 0, ImDrawCornerFlags_All);
            }
            drawList->AddRect(ImVec2(pos.x + key * width + width * 3 / 4, pos.y),
                                    ImVec2(pos.x + key * width + width * 5 / 4 + 1, pos.y + 80),
                              m_synthColors[BLACK], 0, ImDrawCornerFlags_All, 2.0f);
            curKey += 2;
        } else {
            curKey++;
        }
    }
    
    ImGui::End();
}

void Keyboard::keyUp(int key)
{
    m_keyState[key] = { 0 };
}

/*
 A - PINK
 B - BLUE
 C - YELLOW
 D - RED
 E - GREEN
 F - AQUA
 G - ORANGE
 */
void Keyboard::keyDown(int key, int velocity)
{
    if (m_keyState[key][0]) return;
    
//    m_keyState[key] = { 0, m_pressedColors[randomColor] };
//    if (Keyboard::hasBlack(key)) m_keyState[key] = m_pressedColors[1];
//    else m_keyState[key] = m_pressedColors[0];
    int keyRemainder = key % 12;
    switch (keyRemainder) {
        case 0:
            m_keyState[key][0] = m_synthColors[PINK];
            break;
        case 1:
            m_keyState[key][0] = m_synthColors[PINK];
            m_keyState[key][1] = m_synthColors[BLUE];
            break;
        case 2:
            m_keyState[key][0] = m_synthColors[BLUE];
            break;
        case 3:
            m_keyState[key][0] = m_synthColors[YELLOW];
            break;
        case 4:
            m_keyState[key][0] = m_synthColors[YELLOW];
            m_keyState[key][1] = m_synthColors[RED];
            break;
        case 5:
            m_keyState[key][0] = m_synthColors[RED];
            break;
        case 6:
            m_keyState[key][0] = m_synthColors[RED];
            m_keyState[key][1] = m_synthColors[GREEN];
            break;
        case 7:
            m_keyState[key][0] = m_synthColors[GREEN];
            break;
        case 8:
            m_keyState[key][0] = m_synthColors[AQUA];
            break;
        case 9:
            m_keyState[key][0] = m_synthColors[AQUA];
            m_keyState[key][1] = m_synthColors[ORANGE];
            break;
        case 10:
            m_keyState[key][0] = m_synthColors[ORANGE];
            break;
        case 11:
            m_keyState[key][0] = m_synthColors[ORANGE];
            m_keyState[key][1] = m_synthColors[PINK];
            break;
            
        default:
            break;
    }
//    if (key % 12 == 0) m_keyState[key] = m_synthColors[PINK];
//    else if (key)
}

std::vector<int> Keyboard::getKeys()
{
    std::vector<int> result {};
    for (int i = 0; i < 256; i++) {
        if (m_keyState[i][0])
            result.push_back(i);
    }
    
    return result;
}
