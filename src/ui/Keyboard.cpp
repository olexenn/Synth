//
//  Keyboard.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include <imgui.h>

#include "ui/Keyboard.h"

void Keyboard::draw()
{
    ImU32 Black = IM_COL32(0, 0, 0, 255);
    ImU32 White = IM_COL32(255, 255, 255, 255);
    ImU32 Red = IM_COL32(0, 255, 0, 255);
    ImGui::Begin("Keyboard");
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    int width = 20;
    int curKey = 0;
    
    for (int key = 0; key < 52; key++) {
        ImU32 col = White;
        if (m_keyState[curKey]) col = Red;
        drawList->AddRectFilled(ImVec2(pos.x + key * width, pos.y), ImVec2(pos.x + key * width + width, pos.y + 120), col, 0, ImDrawCornerFlags_All);
        drawList->AddRect(ImVec2(pos.x + key * width, pos.y), ImVec2(pos.x + key * width + width, pos.y + 120), Black, 0, ImDrawCornerFlags_All);
        curKey++;
        if (Keyboard::hasBlack(key))
            curKey++;
    }
    
    curKey = 1;
    for (int key = 0; key < 52; key++) {
        if (Keyboard::hasBlack(key)) {
            ImU32 col = Black;
            if (m_keyState[curKey]) col = Red;
            drawList->AddRectFilled(ImVec2(pos.x + key * width + width * 3 / 4, pos.y),
                                    ImVec2(pos.x + key * width + width * 5 / 4 + 1, pos.y + 80),
                                    col, 0, ImDrawCornerFlags_All);
            drawList->AddRect(ImVec2(pos.x + key * width + width * 3 / 4, pos.y),
                                    ImVec2(pos.x + key * width + width * 5 / 4 + 1, pos.y + 80),
                                    Black, 0, ImDrawCornerFlags_All);
            curKey += 2;
        } else {
            curKey++;
        }
    }
    
    ImGui::End();
}

void Keyboard::keyUp(int key)
{
    m_keyState[key] = 0;
}

void Keyboard::keyDown(int key, int velocity)
{
    m_keyState[key] = velocity;
}

std::vector<int> Keyboard::getKeys()
{
    std::vector<int> result {};
    for (int i = 0; i < 256; i++) {
        if (m_keyState[i])
            result.push_back(i);
    }
    
    return result;
}
