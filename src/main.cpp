#include "ui/Gui.h"
#include "audio/Audio.h"

int main(void) {
    // Audio Init
    Audio audio;

    audio.open(Pa_GetDefaultOutputDevice());
    
    audio.start();
    
    // GUI
    Gui gui(&audio);
    
    gui.run();
    
    return 0;
}
