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

double Voice::getSample(double time, WaveType *type)
{
    double amplitude = m_adsrEnvelope.getAmplitude(time);
    double sample = 0.0f;
    switch (*type) {
        case SINE:
            sample = std::sin(M_PI * 2 * m_frequency * time);
            break;
        case SQUARE:
            sample = 0.8 * (std::sin(2 * M_PI * m_frequency * time) > 0 ? 1.0 : -1.0);
            break;
        case SAW:
        {
            // analog saw
            //            float output = 0.0f;
            //
            //            for (float n = 1.0f; n < 40.0f; n++) {
            //                output += (std::sin(n * 2 * M_PI * m_frequency * time)) / n;
            //            }
            //            sample = 0.9 * output * (2.0f / M_PI);
            
            // digital saw
            sample = (2.0 / M_PI) * (m_frequency * M_PI * std::fmod(time, 1.0 / m_frequency) - (M_PI / 2.0));
            break;
        }
        case TRIANGLE:
            sample = std::asin(std::sin(2 * M_PI * m_frequency * time) * (2.0 / M_PI));
            break;
        default:
            sample = 0.0;
    }
    
    if (m_adsrEnvelope.isNoteOff() && m_adsrEnvelope.getCurrentAmplitude() == 0.0f)
        m_active = false;
    return amplitude * sample;
//    return sample;
}

bool Voice::isActive()
{
    return m_active;
}

void Voice::noteOn(int key, double time)
{
    m_active = true;
    m_frequency = calculateFrequency(key);
    m_adsrEnvelope.noteOn(time);
}

void Voice::noteOff(double time)
{
    m_adsrEnvelope.noteOff(time);
}

double Voice::getFrequency()
{
    return m_frequency;
}

void Voice::reset()
{
    m_adsrEnvelope.reset();
}

// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
double Voice::calculateFrequency(int key)
{
    double a = 1.05946309435;
    double hz = std::pow(a, key - 48); // A3 note as base
    return 220.0 * hz; // base note frequency
}
