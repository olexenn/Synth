//
//  Oscillator.h
//  synth
//
//  Created by Алексей Дудник on 29.10.2022.
//

#ifndef Oscillator_h
#define Oscillator_h

#include "Envelope.h"

enum WaveType
{
    SINE,
    SQUARE,
    TRIANGLE,
    SAW,
};

class Oscillator
{
public:
    float getSample(float frequency, float time);
    
    void noteOn(float time);
    void noteOff(float time);
    
private:
    float m_timeOn;
    float m_timeOff;
    bool m_active;
    WaveType m_type;
    Envelope m_adsrEnvelope;
};

#endif /* Oscillator_h */
