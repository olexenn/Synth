//
//  Synth.cpp
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//

#include <cmath>
#include <iostream>
#include <algorithm>

#include <imgui.h>
#include <imgui-knobs.h>

#include "Synth.h"

Synth::Synth()
: m_attackTime(0.1f)
, m_decayTime(0.01f)
, m_sustainAmplitude(0.8f)
, m_releaseTime(0.2f)
, m_polyphonyCounter(0)
, m_type(SINE)
, m_gain(0.5f)
{
    for (int i = 0; i < NumberOfVoices; i++) {
        Envelope *pAdsrEnvelope = &m_adsrEnvelope;
        Voice *voice = new Voice(&m_attackTime, &m_decayTime, &m_sustainAmplitude, &m_releaseTime);
        m_voices[i] = voice;
    }
    
}

float Synth::getSample(float time)
{
    float sample = 0.0f;
    for (const auto &voice : m_voices) {
        if (voice->isActive())
            sample += voice->getSample(time, &m_type);
    }
    
    return m_gain * sample;
}

void Synth::noteOn(int key, float time)
{
    Voice *voice = findFreeVoice(key);
    
    if (voice) {
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

void Synth::draw()
{
    ImGui::Begin("ADSR");
    
    ImGuiKnobs::Knob("Attack", &m_attackTime, 0.00000001f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &m_decayTime, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &m_sustainAmplitude, 0.00000001f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &m_releaseTime, 0.00000001f, 2.0f);
    
    ImGui::End();
    
    ImGui::Begin("Oscillator");
    
    WaveType selected = m_type;
    
    ImGuiKnobs::Knob("Gain", &m_gain, 0.01f, 1.0f);
//    ImGui::SameLine();
    
    if (ImGui::Selectable("Sine", selected == SINE)) m_type = SINE;
    if (ImGui::Selectable("Square", selected == SQUARE)) m_type = SQUARE;
    if (ImGui::Selectable("Saw", selected == SAW)) m_type = SAW;
    if (ImGui::Selectable("Triangle", selected == TRIANGLE)) m_type = TRIANGLE;
    
    ImGui::End();
}

std::vector<float> Synth::getFrequencies()
{
    std::vector<float> frequencies;
    for (const auto &voice : m_voices) {
        if (voice->isActive())
            frequencies.push_back(voice->getFrequency());
    }
    return frequencies;
}
