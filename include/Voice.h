//
//  Voice.h
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//

#ifndef Voice_h
#define Voice_h

#include <memory>

#include "Envelope.h"

// Single note sound
class Voice
{
public:
//    Voice(Envelope *adsrEnvelope);
    Voice(float *attackTime, float *decayTime, float *sustainAmplitude, float *releaseTime);
    
    float getSample(float time);
    
    bool isActive();
    
    void noteOn(int key, float time);
    
    void noteOff(float time);
    
    float getFrequency();
    
    void reset();
    
    float calculateFrequency(int key);
private:
    float m_frequency;
    float m_timeOn;
    float m_timeOff;
    bool m_active;
    Envelope m_adsrEnvelope;
    
};

#endif /* Voice_h */
