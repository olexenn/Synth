//
//  Gui.cpp
//  synth
//
//  Created by Алексей Дудник on 20.12.2022.
//

#include <imgui.h>
#include <imgui-knobs.h>

#include <ui/Gui.h>

void Gui::keyUp(int key)
{
    m_keyboard.keyUp(key);
}

void Gui::keyDown(int key, int velocity)
{
    m_keyboard.keyDown(key, velocity);
}

void Gui::draw()
{
    
}
