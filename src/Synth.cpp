//
//  Synth.cpp
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//

#include <cmath>
#include <iostream>
#include <algorithm>

#include "Synth.h"

// TODO: Fix Polyphony

Synth::Synth()
{
    m_polyphonyCounter = 0;
    
    for (int i = 0; i < NumberOfVoices; i++) {
        Envelope *pAdsrEnvelope = &m_adsrEnvelope;
        Voice* voice = new Voice(pAdsrEnvelope);
        m_voices[i] = voice;
    }
    
}

float Synth::getSample(float time)
{
    float sample = 0.0f;
    for (const auto &voice : m_voices) {
        if (voice->isActive())
            sample += voice->getSample(time);
    }
    
    return sample;
}

void Synth::noteOn(int key, float time)
{
    Voice *voice = findFreeVoice(key);
    
    if (voice) {
//        std::cout << "Note" << std::endl;
        m_polyphonyCounter++;
        voice->reset();
        voice->noteOn(key, time);
    }
}

void Synth::noteOff(int key, float time)
{
    std::cout << "Note Off at: " << time << std::endl;
    m_polyphonyCounter--;
        for (const auto &voice: m_voices) {
            if (voice->isActive() && voice->getFrequency() == voice->calculateFrequency(key)) {
                voice->noteOff(time);
            }
        }
}

void Synth::draw()
{
    m_adsrEnvelope.draw();
}
    
int Synth::getCounter()
{
    return m_polyphonyCounter;
}

Voice* Synth::findFreeVoice(int key)
{
    Voice *freeVoice = nullptr;
    
    for (auto &voice : m_voices) {
        if (voice->getFrequency() == voice->calculateFrequency(key) && voice->isActive())
            return nullptr;
    }
    
    for (auto &voice : m_voices) {
        if (!(voice->isActive())) {
            freeVoice = voice;
            break;
        }
    }
    
    for (int i = 0; i < NumberOfVoices; i++) {
        std::cout << "voice[" << i << "] = " << m_voices[i]->getFrequency() << " activity: " << m_voices[i]->isActive() << std::endl;
    }
    
    return freeVoice;
}
