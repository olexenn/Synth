//
//  Synth.cpp
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//
#include <cmath>
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
, m_osc1Type(SINE)
, m_osc2Type(SINE)
, m_osc3Type(SINE)
, m_osc1Gain(0.5f)
, m_osc2Gain(0.5f)
, m_osc3Gain(0.5f)
, m_isOsc1Active(true)
, m_isOsc2Active(false)
, m_isOsc3Active(false)
, m_filterType(LOW_PASS)
, m_isFilter(false)
, m_filterLowCuttoff(19000.0f)
, m_filterHighCuttoff(100.0f)
, m_isLfoActive(false)
, m_lfoFrequency(5.0f)
, m_osc1NoteOffset(0)
, m_osc2NoteOffset(0)
, m_osc3NoteOffset(0)
{
    for (int i = 0; i < NumberOfVoices; i++) {
        auto voice = std::make_shared<Voice>(&m_attackTime, &m_decayTime, &m_sustainAmplitude, &m_releaseTime, &m_filterLowCuttoff, &m_filterHighCuttoff, &m_filterType, &m_isOsc1Active, &m_osc1Type, &m_osc1Gain, &m_isOsc2Active, &m_osc2Type, &m_osc2Gain, &m_isOsc3Active, &m_osc3Type, &m_osc3Gain, &m_lfoFrequency, &m_isLfoActive, &m_osc1NoteOffset, &m_osc2NoteOffset, &m_osc3NoteOffset, &m_isFilter);
        m_voices[i] = voice;
    }
    
}

double Synth::getSample(double time)
{
    double sample = 0.0;
    for (const auto &voice : m_voices) {
        if (voice->isActive())
            sample += voice->getSample(time);
    }
    
    return sample;
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
    
    return freeVoice;
}

void Synth::draw()
{
    ImGui::Begin("ADSR");
    
    ImGuiKnobs::Knob("Attack", &m_attackTime, 0.001f, 1.0f, 0.01f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &m_decayTime, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &m_sustainAmplitude, 0.01f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &m_releaseTime, 0.01f, 2.0f, 0.01f);
    
    ImGui::End();
    
    ImGui::Begin("Oscillator 1");
    
    ImGuiKnobs::Knob("Gain", &m_osc1Gain, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::KnobInt("Offset", &m_osc1NoteOffset, 0, 48, 12);
    ImGui::SameLine();
    ImGui::Checkbox("Active", &m_isOsc1Active);
    
    if (ImGui::Selectable("Sine", m_osc1Type == SINE)) m_osc1Type = SINE;
    if (ImGui::Selectable("Square", m_osc1Type == SQUARE)) m_osc1Type = SQUARE;
    if (ImGui::Selectable("Saw", m_osc1Type == SAW)) m_osc1Type = SAW;
    if (ImGui::Selectable("Triangle", m_osc1Type == TRIANGLE)) m_osc1Type = TRIANGLE;
    if (ImGui::Selectable("Noise", m_osc1Type == NOISE)) m_osc1Type = NOISE;
    
    ImGui::End();
    ImGui::Begin("Oscillator 2");
    
    ImGuiKnobs::Knob("Gain", &m_osc2Gain, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::KnobInt("Offset", &m_osc2NoteOffset, 0, 48, 12);
    ImGui::SameLine();
    ImGui::Checkbox("Active", &m_isOsc2Active);
    
    if (ImGui::Selectable("Sine", m_osc2Type == SINE)) m_osc2Type = SINE;
    if (ImGui::Selectable("Square", m_osc2Type == SQUARE)) m_osc2Type = SQUARE;
    if (ImGui::Selectable("Saw", m_osc2Type == SAW)) m_osc2Type = SAW;
    if (ImGui::Selectable("Triangle", m_osc2Type == TRIANGLE)) m_osc2Type = TRIANGLE;
    if (ImGui::Selectable("Noise", m_osc2Type == NOISE)) m_osc2Type = NOISE;
    
    ImGui::End();
    ImGui::Begin("Oscillator3");
    ImGuiKnobs::Knob("Gain", &m_osc3Gain, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::KnobInt("Offset", &m_osc3NoteOffset, 0, 48, 12);
    ImGui::SameLine();
    ImGui::Checkbox("Active", &m_isOsc3Active);
    
    if (ImGui::Selectable("Sine", m_osc3Type == SINE)) m_osc3Type = SINE;
    if (ImGui::Selectable("Square", m_osc3Type == SQUARE)) m_osc3Type = SQUARE;
    if (ImGui::Selectable("Saw", m_osc3Type == SAW)) m_osc3Type = SAW;
    if (ImGui::Selectable("Triangle", m_osc3Type == TRIANGLE)) m_osc3Type = TRIANGLE;
    if (ImGui::Selectable("Noise", m_osc3Type == NOISE)) m_osc3Type = NOISE;
    
    ImGui::End();
    
    ImGui::Begin("Filter");
    
    ImGui::Checkbox("Active", &m_isFilter);
    
    if (ImGui::Selectable("LowPass", m_filterType == LOW_PASS)) m_filterType = LOW_PASS;
    if (ImGui::Selectable("HighPass", m_filterType == HIGH_PASS)) m_filterType = HIGH_PASS;
    
    ImGuiKnobs::Knob("Low Pass", &m_filterLowCuttoff, 100.0f, 20000.0f, 20.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("High Pass", &m_filterHighCuttoff, 100.0f, 3000.0f, 20.0f);
    
    ImGui::End();
    
    ImGui::Begin("LFO");
    
    ImGui::Checkbox("Active", &m_isLfoActive);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Freq", &m_lfoFrequency, 1.0f, 100.0f, 1.0f);
    
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
