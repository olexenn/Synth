//
//  Synth.h
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//

#ifndef Synth_h
#define Synth_h

#include <vector>
#include <array>
#include <memory>

#include "Envelope.h"
#include "Voice.h"

// Main sound machine
class Synth {
public:
    static const int NumberOfVoices = 20; // max polyphony is 20 notes
    Synth();
    
    float getSample(float time);
    
    void noteOn(int key, float time);
    void noteOff(int key, float time);
    
    int getCounter();
    
    void draw();
  
private:
    int m_polyphonyCounter;
    std::array<Voice*, NumberOfVoices> m_voices;
    Envelope m_adsrEnvelope;
    
    Voice* findFreeVoice(int key);
    
    void removeVoice(Voice *voice);
    
};

#endif /* Synth_h */
