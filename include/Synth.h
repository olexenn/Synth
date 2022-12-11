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
    
    const std::array<Voice*, NumberOfVoices>& getVoices();
    
private:
    int m_polyphonyCounter = 0;
    
//    Envelope m_adsrEnvelope;
//    std::unique_ptr<Envelope> m_adsrEnvelope;
    std::unique_ptr<Envelope> m_adsrEnvelope = std::make_unique<Envelope>();
    AdsrParams m_adsrParams;
    
    Filter m_filter;
    
    std::array<Voice*, NumberOfVoices> m_voices;
    
    bool m_isOsc1Active = true;
    WaveType m_osc1Type = SINE;
    float m_osc1Gain = 0.5f;
    int m_osc1NoteOffset = 0;
    
    bool m_isOsc2Active = false;
    WaveType m_osc2Type = SINE;
    float m_osc2Gain = 0.5f;
    int m_osc2NoteOffset = 0;
    
    bool m_isOsc3Active = false;
    WaveType m_osc3Type = SINE;
    float m_osc3Gain = 0.5f;
    int m_osc3NoteOffset = 0;
    
    float m_lfoFrequency = 5.0f;
    bool m_isLfoActive = false;
    
    
    Voice* findFreeVoice(int key);
    
};

#endif /* Synth_h */
