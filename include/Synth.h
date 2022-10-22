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
    
    std::vector<float> getFrequencies();
  
private:
    int m_polyphonyCounter;
    float m_attackTime;
    float m_decayTime;
    float m_sustainAmplitude;
    float m_releaseTime;
    float m_gain;
    
    std::array<Voice*, NumberOfVoices> m_voices;
    Envelope m_adsrEnvelope;
    
    WaveType m_type;
    
    Voice* findFreeVoice(int key);
    
};

#endif /* Synth_h */
