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
, m_filterType(LOW_PASS)
, m_filterLowCuttoff(19000.0f)
, m_filterHighCuttoff(0.0f)
, m_gain(0.5)
{
    for (int i = 0; i < NumberOfVoices; i++) {
        auto voice = std::make_shared<Voice>(&m_attackTime, &m_decayTime, &m_sustainAmplitude, &m_releaseTime, &m_filterLowCuttoff, &m_filterHighCuttoff, &m_filterType);
        m_voices[i] = voice;
    }
    
}

double Synth::getSample(double time)
{
    double sample = 0.0;
    for (const auto &voice : m_voices) {
        if (voice->isActive())
            sample += voice->getSample(time, &m_type);
    }
    
    return m_gain * sample;
}

void Synth::noteOn(int key, double time)
{
    auto voice = findFreeVoice(key);
    
    if (voice) {
        m_polyphonyCounter++;
        voice->reset();
        voice->noteOn(key, time);
    }
}

void Synth::noteOff(int key, double time)
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

std::shared_ptr<Voice> Synth::findFreeVoice(int key)
{
    std::shared_ptr<Voice> freeVoice = nullptr;
    
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
    
    
    ImGuiKnobs::Knob("Gain", &m_gain, 0.01f, 1.0f);
    
    if (ImGui::Selectable("Sine", m_type == SINE)) m_type = SINE;
    if (ImGui::Selectable("Square", m_type == SQUARE)) m_type = SQUARE;
    if (ImGui::Selectable("Saw", m_type == SAW)) m_type = SAW;
    if (ImGui::Selectable("Triangle", m_type == TRIANGLE)) m_type = TRIANGLE;
    
    ImGui::End();
    
    ImGui::Begin("Filter");
    
    if (ImGui::Selectable("LowPass", m_filterType == LOW_PASS)) m_filterType = LOW_PASS;
    if (ImGui::Selectable("HighPass", m_filterType == HIGH_PASS)) m_filterType = HIGH_PASS;
    
    ImGuiKnobs::Knob("Low Pass", &m_filterLowCuttoff, 100.0f, 20000.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("High Pass", &m_filterHighCuttoff, 100.0f, 3000.0f);
    
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
