//
//  Oscillator.cpp
//  synth
//
//  Created by Алексей Дудник on 29.10.2022.
//
#include <cmath>

#include "audio/Oscillator.h"

#ifndef M_PI
#define M_PI  (3.14159265358979323846)
#endif

Oscillator::Oscillator(WaveType *type, bool *isActive, int *noteOffset, float *gain)
{
    m_type = type;
    m_isActive = isActive;
    m_noteOffset = noteOffset;
    m_gain = gain;
}

double Oscillator::getSample(int key, double time, double lfoFrequency)
{
    double sample = 0.0;
    if (!(*m_isActive))
        return sample;
    
    double frequency = calculateFrequency(key);
    double radians = 2 * M_PI * frequency * time + 0.001 * frequency * std::sin(2 * M_PI * lfoFrequency * time);
        
    switch (*m_type) {
        case SINE:
            sample = std::sin(radians);
            break;
        case SQUARE:
            sample = 0.8 * (std::sin(radians) > 0 ? 1.0f : -1.0f);
        break;
        case SAW: {
            double out = 0.0;
            for (double n = 1.0; n < 50.0; n++) {
                out += (std::sin(n * radians)) / n;
            }
            sample = out;
            break;
        }
        case TRIANGLE:
            sample = std::asin(std::sin(radians) * (2.0 / M_PI));
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

// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
double Oscillator::calculateFrequency(int key)
{
    double a = 1.05946309435;
    double hz = std::pow(a, (*m_noteOffset + key) - 48); // A3 note as base
    return 220.0 * hz; // base note frequency
}
