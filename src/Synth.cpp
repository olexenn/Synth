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
{
    m_adsrParams.attackTime = 0.1f;
    m_adsrParams.decayTime = 0.01f;
    m_adsrParams.sustainAmplitude = 0.8f;
    m_adsrParams.releaseTime = 0.2f;
    
    auto pOsc1 = new Oscillator(&m_osc1Type, &m_isOsc1Active,
                                &m_osc1NoteOffset, &m_osc1Gain);
    auto pOsc2 = new Oscillator(&m_osc2Type, &m_isOsc2Active,
                                &m_osc2NoteOffset, &m_osc2Gain);
    auto pOsc3 = new Oscillator(&m_osc3Type, &m_isOsc3Active,
                                &m_osc3NoteOffset, &m_osc3Gain);
    auto pLfo = new Lfo(&m_lfoFrequency, &m_isLfoActive);
    
    for (int i = 0; i < NumberOfVoices; i++) {
        auto voice = new Voice(&m_adsrParams, pOsc1, pOsc2, pOsc3, pLfo);
        m_voices[i] = voice;
    }
    
}

double Synth::getSample(double time)
{
    double sample = 0.0;
    for (auto voice : m_voices) {
        if (voice->isActive()) {
            sample += voice->getSample(time);
            
            if (m_filter.isFilterOn())
                sample = m_filter.getFilteredSample(sample);
        }
    }
    
    return sample;
}

void Synth::noteOn(int key, double time)
{
    auto voice = findFreeVoice(key);
    
    if (voice) {
        m_polyphonyCounter++;
//        voice->reset();
//        m_adsrEnvelope->noteOn(time);
        voice->noteOn(key, time);
    }
}

void Synth::noteOff(int key, double time)
{
        for (auto voice : m_voices) {
            if (voice->isActive() && voice->getKey() == key) {
                m_polyphonyCounter--;
//                m_adsrEnvelope->noteOff(time);
                voice->reset();
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
    for (auto voice : m_voices) {
        if (voice->isActive() && voice->getKey() == key)
            return nullptr;
    }
    
    for (auto &voice : m_voices) {
        if (!(voice->isActive())) {
            return voice;
        }
        
    }
}

const std::array<Voice*, Synth::NumberOfVoices>& Synth::getVoices()
{
    return m_voices;
}

void Synth::draw()
{
    ImGui::Begin("ADSR");
    
    ImGuiKnobs::Knob("Attack", &m_adsrParams.attackTime, 0.001f, 1.0f, 0.01f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &m_adsrParams.decayTime, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &m_adsrParams.sustainAmplitude, 0.01f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &m_adsrParams.releaseTime, 0.01f, 2.0f, 0.01f);
    
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
    
    ImGui::Checkbox("Active", &m_filter.getActivity());
    
    if (ImGui::Selectable("LowPass", m_filter.getFilterType() == LOW_PASS))
        m_filter.setFilterType(LOW_PASS);
    if (ImGui::Selectable("HighPass", m_filter.getFilterType() == HIGH_PASS))
        m_filter.setFilterType(HIGH_PASS);
    
    ImGuiKnobs::Knob("Low Pass", &m_filter.getLowCuttoff(), 100.0f, 20000.0f, 20.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("High Pass", &m_filter.getHighCuttoff(), 100.0f, 3000.0f, 20.0f);
    
    ImGui::End();
    
    ImGui::Begin("LFO");
    
    ImGui::Checkbox("Active", &m_isLfoActive);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Freq", &m_lfoFrequency, 1.0f, 100.0f, 1.0f);
    
    ImGui::End();
}

