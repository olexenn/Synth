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
#include "Filter.h"

// Single note sound
class Voice
{
public:
//    Voice(Envelope *adsrEnvelope);
    Voice(float *attackTime, float *decayTime, float *sustainAmplitude, float *releaseTime, float *filterLowCuttoff, float *filterHighCuttoff, FilterType *filterType, bool *isOsc1Active, WaveType *osc1Type, float *osc1Gain, bool *isOsc2Active, WaveType *osc2Type, float *osc2Gain, bool *isOsc3Active, WaveType *osc3Type, float *osc3Gain, float *lfoFrequency, bool *lfoActive, int *osc1Offset, int *osc2Offset, int *osc3Offset, bool *isFilter);
    
    double getSample(double time);
    
    bool isActive();
    
    void noteOn(int key, double time);
    
    void noteOff(double time);
    
    double getFrequency();
    
    void reset();
    
    double calculateFrequency(int key);
private:
    std::array<Oscillator, 3> m_oscillators;
    double m_frequency;
    int m_key;
    double m_timeOn;
    double m_timeOff;
    bool m_active;
    Envelope m_adsrEnvelope;
    Filter m_filter;
    bool *m_isFilter;
};

#endif /* Voice_h */
