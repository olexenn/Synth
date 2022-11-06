//
//  Voice.h
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//

#ifndef Voice_h
#define Voice_h

#include <array>

#include "Envelope.h"
#include "Oscillator.h"

//enum WaveType
//{
//    SINE,
//    SQUARE,
//    TRIANGLE,
//    SAW,
//};

// Single note sound
class Voice
{
public:
//    Voice(Envelope *adsrEnvelope);
    Voice(float *attackTime, float *decayTime, float *sustainAmplitude, float *releaseTime);
    
    double getSample(double time, WaveType *type);
    
    bool isActive();
    
    void noteOn(int key, double time);
    
    void noteOff(double time);
    
    double getFrequency();
    
    void reset();
    
    double calculateFrequency(int key);
private:
    std::array<Oscillator, 5> m_oscillators;
    double m_frequency;
    double m_timeOn;
    double m_timeOff;
    bool m_active;
    Envelope m_adsrEnvelope;
    
};

#endif /* Voice_h */
