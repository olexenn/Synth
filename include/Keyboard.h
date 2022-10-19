//
//  Keyboard.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Keyboard_h
#define Keyboard_h

#include <array>
#include <vector>

// Keyboard UI
class Keyboard
{
public:
    void draw();
    
    void keyUp(int key);
    
    void keyDown(int key, int velocity);
    
    std::vector<int> getKeys();
    
private:
    std::array<int, 256> m_keyState = { 0 };
    static bool hasBlack(int key)
    {
        return (!((key - 1) % 7 == 0 || (key - 1) % 7 == 3) && key != 51);
    }
};

#endif /* Keyboard_h */
