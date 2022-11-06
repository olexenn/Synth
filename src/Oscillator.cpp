//
//  Oscillator.cpp
//  synth
//
//  Created by Алексей Дудник on 29.10.2022.
//

#include "Oscillator.h"

#include <cmath>

void Oscillator::noteOn(float time)
{
    m_adsrEnvelope.noteOn(time);
}

void Oscillator::noteOff(float time)
{
    m_adsrEnvelope.noteOff(time);
}

float Oscillator::getSample(float frequency, float time)
{
    float amplitude = m_adsrEnvelope.getAmplitude(time);
    float sample = 0.0f;
    switch (m_type) {
        case SINE:
            sample = std::sin(M_PI * 2 * frequency * time);
            break;
        case SQUARE:
            sample = 0.8 * (std::sin(2 * M_PI * frequency * time) > 0 ? 1.0f : -1.0f);
            break;
        case SAW:
            sample = (2.0f / M_PI) * (frequency * M_PI * std::fmod(time, 1.0 / frequency) - (M_PI / 2.0));
            break;
        case TRIANGLE:
            sample = std::asin(std::sin(2 * M_PI * frequency * time) * (2.0 / M_PI));
            break;
        default:
            sample = 0.0f;
            break;
    }
    
    if (m_adsrEnvelope.isNoteOff() && m_adsrEnvelope.getCurrentAmplitude() == 0.0f)
        m_active = false;
    return amplitude * sample;
}
