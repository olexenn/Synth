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
    
    double getSample(double time);
    
    void noteOn(int key, double time);
    void noteOff(int key, double time);
    
    int getCounter();
    
    void draw();
    
    std::vector<float> getFrequencies();
  
private:
    int m_polyphonyCounter;
    float m_attackTime;
    float m_decayTime;
    float m_sustainAmplitude;
    float m_releaseTime;
    
    float m_filterLowCuttoff;
    float m_filterHighCuttoff;
    bool m_isFilter;
    
    std::array<std::shared_ptr<Voice>, NumberOfVoices> m_voices;
    Envelope m_adsrEnvelope;
    
    bool m_isOsc1Active;
    WaveType m_osc1Type;
    float m_osc1Gain;
    int m_osc1NoteOffset;
    
    bool m_isOsc2Active;
    WaveType m_osc2Type;
    float m_osc2Gain;
    int m_osc2NoteOffset;
    
    bool m_isOsc3Active;
    WaveType m_osc3Type;
    float m_osc3Gain;
    int m_osc3NoteOffset;
    
    float m_lfoFrequency;
    bool m_isLfoActive;
    
    FilterType m_filterType;
    
    std::shared_ptr<Voice> findFreeVoice(int key);
    
};

#endif /* Synth_h */
