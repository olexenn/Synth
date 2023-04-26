//
//  Keyboard.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Keyboard_h
#define Keyboard_h

#include <array>
#include <map>
#include <vector>

enum SynthColors
{
    WHITE,
    WHITE_2,
    WHITE_3,
    PINK,
    BLUE,
    YELLOW,
    RED,
    GREEN,
    AQUA,
    ORANGE,
    DARK_BLUE,
    DARK_GREEN,
    DARK_GREEN_1,
    BROWN,
    UNKNOWN_YELLOW,
    BLACK,
    GREY,
    GREY_2,
    GREY_3,
    PINK_BLUE,
    YELLOW_RED,
    RED_GREEN,
    AQUA_ORANGE,
    ORANGE_PINK,
    NUMBER_OF_COLORS
};

// Keyboard UI
class Keyboard
{
public:
    Keyboard(std::array<ImColor, NUMBER_OF_COLORS>& colors);
    void draw();
    
    void keyUp(int key);
    
    void keyDown(int key, int velocity);
    
    std::vector<int> getKeys(); //debug
    
private:
    std::map<int, std::array<ImColor, 2>> m_keyState;
//    std::array<int, 256> m_keyState = { 0 };
    std::array<ImColor, NUMBER_OF_COLORS> m_synthColors;
    std::array<ImColor, 2> m_pressedColors;
//    ImColor m_synthColors[NUMBER_OF_COLORS];
//    ImColor m_pressedColors[6];
    static bool hasBlack(int key)
    {
        return (!((key - 1) % 7 == 0 || (key - 1) % 7 == 3) && key != 51);
    }
};

#endif /* Keyboard_h */
