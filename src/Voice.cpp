//
//  Voice.cpp
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//

#include <cmath>

#include "Voice.h"

Voice::Voice(Envelope *adsrEnvelope)
: m_frequency(0.f)
, m_timeOn(0.f)
, m_timeOff(0.f)
, m_active(false)
, m_adsrEnvelope(adsrEnvelope)
{
}

float Voice::getSample(float time)
{
    float amplitude = m_adsrEnvelope->getAmplitude(time);
    float sample = amplitude * std::sinf(M_PI * 2 * m_frequency * time);
    
    if (m_adsrEnvelope->isNoteOff() && m_adsrEnvelope->getCurrentAmplitude() == 0.0f)
        m_active = false;
    return sample;
}

bool Voice::isActive()
{
    return m_active;
}

void Voice::noteOn(int key, float time)
{
    m_active = true;
    m_frequency = calculateFrequency(key);
    m_adsrEnvelope->noteOn(time);
}

void Voice::noteOff(float time)
{
//    m_frequency = 0.0f;
    m_adsrEnvelope->noteOff(time);
}

float Voice::getFrequency()
{
    return m_frequency;
}

void Voice::reset()
{
//    m_frequency = 0.0f;
//    m_active = false;
    m_adsrEnvelope->reset();
}

// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
float Voice::calculateFrequency(int key)
{
    float a = 1.05946309435;
    float hz = std::pow(a, static_cast<float>(key) - 48); // A3 note as base
    return 220.0f * hz; // base note frequency
}
