#include "ui/Gui.h"
#include "audio/Audio.h"

#define UI_DEBUG 0

int main(void) {
#if UI_DEBUG
    Gui gui;
    gui.testRun();
#else
    // Audio Init
    Audio audio;

    audio.open(Pa_GetDefaultOutputDevice());
    
    audio.start();
    
    // GUI
    Gui gui(&audio);
    
    gui.run();
    
#endif
    
    return 0;
}
