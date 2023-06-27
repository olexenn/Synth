#include <iostream>
#include "ui/Gui.h"
#include "audio/Audio.h"

#define UI_DEBUG 0

int main(void) {
#if UI_DEBUG
    Gui gui;
    gui.testRun();
#else
    const PaVersionInfo* govno = Pa_GetVersionInfo();
    std::cout << "version: " << govno->versionText << std::endl;
    // Audio Init
    Audio audio;

    std::cout << "Created audio object" << std::endl;

    audio.open(Pa_GetDefaultOutputDevice());

    audio.start();
 
    // GUI
    Gui gui(&audio);
    
    gui.run();
    
#endif
    
    return 0;
}
