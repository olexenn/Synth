//
//  Voice.cpp
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//
#include <cmath>

#include "audio/Voice.h"

#ifndef M_PI
#define M_PI  (3.14159265358979323846)
#endif

Voice::Voice(Envelope *pEnv, Oscillator *osc1, Oscillator *osc2,
             Oscillator *osc3, Lfo *lfo)
{
    m_adsr = pEnv;
    
    m_oscillators[0] = osc1;
    m_oscillators[1] = osc2;
    m_oscillators[2] = osc3;
    m_lfo = lfo;
}

double Voice::getSample(double time)
{
    double amplitude = m_adsr->getAmplitude(time);
    
    double sample = 0.0;
    double lfoFreq = 0.0;
    
    for (int i = 0; i < m_oscillators.size(); i++) {
        if (i == 0 && m_lfo->isActive()) lfoFreq = m_lfo->getFrequency();
        sample += m_oscillators[i]->getSample(m_key, time, lfoFreq);
    }
    
    if (m_adsr->isNoteOff() && m_adsr->getCurrentAmplitude() == 0.0)
        m_isActive = false;
    
    return amplitude * sample;
}

bool Voice::isActive()
{
    return m_isActive;
}

void Voice::noteOn(int key, double time)
{
    m_adsr->noteOn(time);
    m_isActive = true;
    m_key = key;
}

void Voice::noteOff(double time)
{
    m_adsr->noteOff(time);
}

int Voice::getKey()
{
    return m_key;
}
