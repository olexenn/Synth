//
//  Oscillator.cpp
//  synth
//
//  Created by Алексей Дудник on 29.10.2022.
//

#include <cmath>

#include <imgui.h>
#include <imgui-knobs.h>

#include "Oscillator.h"

double Oscillator::getSample(int key, double time)
{
    double sample = 0.0;
    if (!(*m_isActive))
        return sample;
    
    double frequency = calculateFrequency(key);
    if (m_isLfoActive && *m_isLfoActive)
        frequency = 2 * M_PI * frequency * time + 0.001 * frequency * std::sin(2 * M_PI * *m_lfoFrequency * time);
    else
        frequency = 2 * M_PI * frequency * time;
        
    switch (*m_type) {
        case SINE:
            sample = std::sin(frequency);
            break;
        case SQUARE:
            sample = 0.8 * (std::sin(frequency) > 0 ? 1.0f : -1.0f);
        break;
        case SAW: {
            double out = 0.0;
            for (double n = 1.0; n < 50.0; n++) {
                out += (std::sin(n * frequency)) / n;
            }
            sample = out;
            break;
        }
        case TRIANGLE:
            sample = std::asin(std::sin(frequency) * (2.0 / M_PI));
            break;
        case NOISE:
            sample = 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
            break;
        default:
            sample = 0.0f;
            break;
    }
    
    return *m_gain * sample;
}

void Oscillator::setType(WaveType *type)
{
    m_type = type;
}

void Oscillator::setGain(float *gain)
{
    m_gain = gain;
}

void Oscillator::setActive(bool *isOscActive)
{
    m_isActive = isOscActive;
}

void Oscillator::setLfoActivity(bool *isLfoActive)
{
    m_isLfoActive = isLfoActive;
}

void Oscillator::setLfoFrequency(float *frequency)
{
    m_lfoFrequency = frequency;
}

void Oscillator::setNoteOffset(int *offset)
{
    m_noteOffset = offset;
}
// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
double Oscillator::calculateFrequency(int key)
{
    double a = 1.05946309435;
    double hz = std::pow(a, (*m_noteOffset + key) - 48); // A3 note as base
    return 220.0 * hz; // base note frequency
}
