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

class Gui
{
public:
    Gui(Audio *audio);
    ~Gui();
    void run();
    
private:
    GLFWwindow *m_window;
    ImGuiStyle m_style;
    Keyboard m_keyboard;
    Audio *m_audio;
    
    void noteOff(int key);
    void noteOn(int key, int velocity);
    void draw();
};

#endif /* Gui_h */
