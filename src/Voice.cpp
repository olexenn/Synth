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

Voice::Voice(float *attackTime, float *decayTime, float *sustainAmplitude, float *releaseTime, float *filterLowCuttoff, float *filterHighCuttoff, FilterType *filterType, bool *isOsc1Active, WaveType *osc1Type, float *osc1Gain, bool *isOsc2Active, WaveType *osc2Type, float *osc2Gain, bool *isOsc3Active, WaveType *osc3Type, float *osc3Gain, float *lfoFrequency, bool *lfoActive, int *osc1Offset, int *osc2Offset, int *osc3Offset, bool *isFilter)
: m_frequency(0.f)
, m_timeOn(0.f)
, m_timeOff(0.f)
, m_active(false)
, m_key(0)
, m_isFilter(isFilter)
{
    m_adsrEnvelope.setAttackTime(attackTime);
    m_adsrEnvelope.setDecayTime(decayTime);
    m_adsrEnvelope.setSustainAmplitude(sustainAmplitude);
    m_adsrEnvelope.setReleaseTime(releaseTime);
    
    m_filter.setLowCuttoff(filterLowCuttoff);
    m_filter.setHighCuttoff(filterHighCuttoff);
    m_filter.setFilterType(filterType);
    
    m_oscillators[0].setActive(isOsc1Active);
    m_oscillators[0].setType(osc1Type);
    m_oscillators[0].setGain(osc1Gain);
    m_oscillators[0].setNoteOffset(osc1Offset);
    m_oscillators[1].setActive(isOsc2Active);
    m_oscillators[1].setType(osc2Type);
    m_oscillators[1].setGain(osc2Gain);
    m_oscillators[1].setNoteOffset(osc2Offset);
    m_oscillators[2].setActive(isOsc3Active);
    m_oscillators[2].setType(osc3Type);
    m_oscillators[2].setGain(osc3Gain);
    m_oscillators[2].setNoteOffset(osc3Offset);
    m_oscillators[0].setLfoActivity(lfoActive);
    m_oscillators[0].setLfoFrequency(lfoFrequency);
    for (int i = 1; i < m_oscillators.size(); i++) {
        m_oscillators[i].setLfoActivity(nullptr);
        m_oscillators[i].setLfoFrequency(nullptr);
    }
}


double Voice::getSample(double time)
{
    double amplitude = m_adsrEnvelope.getAmplitude(time);
    double sample = 0.0f;
    
    for (int i = 0; i < m_oscillators.size(); i++) {
        sample += m_oscillators[i].getSample(m_key, time);
    }
    
    if (*m_isFilter)
        sample = m_filter.getFilteredSample(sample);
    
    if (m_adsrEnvelope.isNoteOff() && m_adsrEnvelope.getCurrentAmplitude() == 0.0f)
        m_active = false;
    return amplitude * sample;
}

bool Voice::isActive()
{
    return m_active;
}

void Voice::noteOn(int key, double time)
{
    m_active = true;
    m_frequency = calculateFrequency(key);
    m_key = key;
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
