//
//  Gui.h
//  synth
//
//  Created by Алексей Дудник on 20.12.2022.
//

#ifndef Gui_h
#define Gui_h

#include <ui/Keyboard.h>

class Gui
{
public:
    Gui() = default;
    void draw();
    void keyUp(int key);
    void keyDown(int key, int velocity);
private:
    Keyboard m_keyboard;
};

#endif /* Gui_h */
