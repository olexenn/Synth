//
//  Voice.cpp
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//

#include <cmath>

#include "Voice.h"

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

Voice::Voice(float *attackTime, float *decayTime, float *sustainAmplitude, float *releaseTime)
: m_frequency(0.f)
, m_timeOn(0.f)
, m_timeOff(0.f)
, m_active(false)
{
    m_adsrEnvelope.m_attackTime = attackTime;
    m_adsrEnvelope.m_decayTime = decayTime;
    m_adsrEnvelope.m_sustainAmplitude = sustainAmplitude;
    m_adsrEnvelope.m_releaseTime = releaseTime;
}

float Voice::getSample(float time, WaveType *type)
{
    float amplitude = m_adsrEnvelope.getAmplitude(time);
    float sample = 0.0;
    switch (*type) {
        case SINE:
            sample = std::sin(M_PI * 2 * m_frequency * time);
            break;
        case SQUARE:
            sample = std::sin(2 * M_PI * m_frequency * time) > 0 ? 1.0 : -1.0;
            break;
        case SAW:
        {
            float output = 0.0f;
            
            for (float n = 1.0f; n < 40.0f; n++) {
                output += (std::sin(n * 2 * M_PI * m_frequency * time)) / n;
            }
            sample = output * (2.0f / M_PI);
            break;
        }
        case TRIANGLE:
            sample = std::asin(std::sin(2 * M_PI * m_frequency * time) * (2.0 / M_PI));
            break;
        default:
            sample = 0.0;
    }
//    float sample = amplitude * std::sinf(M_PI * 2 * m_frequency * time);
    
    if (m_adsrEnvelope.isNoteOff() && m_adsrEnvelope.getCurrentAmplitude() == 0.0f)
        m_active = false;
    return amplitude * sample;
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
    m_adsrEnvelope.noteOn(time);
}

void Voice::noteOff(float time)
{
    m_adsrEnvelope.noteOff(time);
}

float Voice::getFrequency()
{
    return m_frequency;
}

void Voice::reset()
{
    m_adsrEnvelope.reset();
}

// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
float Voice::calculateFrequency(int key)
{
    float a = 1.05946309435;
    float hz = std::pow(a, static_cast<float>(key) - 48); // A3 note as base
    return 220.0f * hz; // base note frequency
}
