//
//  Synth.cpp
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <imgui.h>
#include <imgui-knobs.h>

#include "Synth.h"

Synth::Synth()
{
    
//    m_adsrParams.attackTime = 0.1f;
//    m_adsrParams.decayTime = 0.01f;
//    m_adsrParams.sustainAmplitude = 0.8f;
//    m_adsrParams.releaseTime = 0.2f;
    
    getAllPresets();
    loadPreset(m_presets[0]);
    
    auto pOsc1 = new Oscillator(&m_osc1Type, &m_isOsc1Active,
                                &m_osc1NoteOffset, &m_osc1Gain);
    auto pOsc2 = new Oscillator(&m_osc2Type, &m_isOsc2Active,
                                &m_osc2NoteOffset, &m_osc2Gain);
    auto pOsc3 = new Oscillator(&m_osc3Type, &m_isOsc3Active,
                                &m_osc3NoteOffset, &m_osc3Gain);
    auto pLfo = new Lfo(&m_lfoFrequency, &m_isLfoActive);
    
    
    for (int i = 0; i < NumberOfVoices; i++) {
        auto pEnv = new Envelope(&m_adsrParams.attackTime, &m_adsrParams.decayTime,
                             &m_adsrParams.sustainAmplitude, &m_adsrParams.releaseTime);
        auto voice = new Voice(pEnv, pOsc1, pOsc2, pOsc3, pLfo);
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
        voice->noteOn(key, time);
    }
}

void Synth::noteOff(int key, double time)
{
        for (auto voice : m_voices) {
            if (voice->isActive() && voice->getKey() == key) {
                m_polyphonyCounter--;
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

void Synth::draw(ImGuiStyle& style)
{
    
    // ADSR
    ImGui::Begin("ADSR");
    ImGuiKnobs::Knob("Attack", &m_adsrParams.attackTime, 0.001f, 1.0f, 0.01f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &m_adsrParams.decayTime, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &m_adsrParams.sustainAmplitude, 0.0f, 1.0f, 0.05f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &m_adsrParams.releaseTime, 0.01f, 1.0f, 0.01f);
    ImGui::End();
    
    // OSC 1
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
    
    // OSC2
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
    
    // OSC 3
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
    
    // FILTER
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
    
    // LFO
    ImGui::Begin("LFO");
    ImGui::Checkbox("Active", &m_isLfoActive);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Freq", &m_lfoFrequency, 1.0f, 100.0f, 1.0f);
    ImGui::End();
    
    // PRESET MANAGER
    ImGui::Begin("Presets");
    static const char *currentItem = m_presets[0].c_str();
    auto numberOfPrestes = m_presets.size();
//    if (numberOfPrestes > 0)
//        currentItem = m_presets[0].c_str();
    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemSpacing.x;
    float buttonSz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - buttonSz * 2.0f);
    if (ImGui::BeginCombo("Presets", currentItem, ImGuiComboFlags_NoArrowButton)) {
        for (int i = 0; i < numberOfPrestes; i++) {
            bool isSelected = (currentItem == m_presets[i]);
            if (ImGui::Selectable(m_presets[i].c_str())) {
                currentItem = m_presets[i].c_str();
                loadPreset(currentItem);
                break;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##l", ImGuiDir_Left)) {
        if (--m_counter < 0)
            m_counter = numberOfPrestes - 1;
        currentItem = m_presets[m_counter].c_str();
        loadPreset(currentItem);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::ArrowButton("##r", ImGuiDir_Right)) {
        if (++m_counter == numberOfPrestes)
            m_counter = 0;
        currentItem = m_presets[m_counter].c_str();
        loadPreset(currentItem);
    }
    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    if (ImGui::Button("Save")) ImGui::OpenPopup("Save new preset");
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal("Save new preset", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char buf[64] = "";
        ImGui::Text("Enter the name for new preset");
        ImGui::InputText("Name", buf, 64);
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (std::strcmp(buf, "") || std::strcmp(buf, " ")) ImGui::CloseCurrentPopup();
            savePreset(buf);
            getAllPresets();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    ImGui::End();
}

void Synth::savePreset(std::string name)
{
    name += ".preset";
    std::ofstream preset;
    preset.open("presets/" + name);
    if (!preset)
        exit(1);
    
    // ADSR
    preset << m_adsrParams.attackTime << std::endl;
    preset << m_adsrParams.decayTime << std::endl;
    preset << m_adsrParams.sustainAmplitude << std::endl;
    preset << m_adsrParams.releaseTime << std::endl;
    
    // FILTER
    preset << m_filter.getFilterType() << std::endl;
    preset << m_filter.getLowCuttoff() << std::endl;
    preset << m_filter.getHighCuttoff() << std::endl;
    preset << m_filter.getActivity() << std::endl;
    
    // OSC1
    preset << m_isOsc1Active << std::endl;
    preset << m_osc1Type << std::endl;
    preset << m_osc1Gain << std::endl;
    preset << m_osc1NoteOffset << std::endl;
    
    // OSC2
    preset << m_isOsc2Active << std::endl;
    preset << m_osc2Type << std::endl;
    preset << m_osc2Gain << std::endl;
    preset << m_osc2NoteOffset << std::endl;
    
    // OSC3
    preset << m_isOsc3Active << std::endl;
    preset << m_osc3Type << std::endl;
    preset << m_osc3Gain << std::endl;
    preset << m_osc3NoteOffset << std::endl;
    
    // LFO
    preset << m_lfoFrequency << std::endl;
    preset << m_isLfoActive << std::endl;
    
    preset.close();
}

void Synth::loadPreset(std::string name)
{
    std::string fullName = name + ".preset";
    std::ifstream preset;
    //std::cout << fullName << std::endl;
    //std::cin.get();
    preset.open("presets/" + fullName);
    if (!preset) {
        std::cerr << "ERROR: Couldn't open preset for reading\n";
        exit(1);
    }
    
    // ADSR
    preset >> m_adsrParams.attackTime;
    preset >> m_adsrParams.decayTime;
    preset >> m_adsrParams.sustainAmplitude;
    preset >> m_adsrParams.releaseTime;
    
    // FILTER
    int filterType;
    preset >> filterType;
    m_filter.setFilterType(static_cast<FilterType>(filterType));
    preset >> m_filter.getLowCuttoff();
    preset >> m_filter.getHighCuttoff();
    preset >> m_filter.getActivity();
    
    // OSC 1
    preset >> m_isOsc1Active;
    int osc1Type;
    preset >> osc1Type;
    m_osc1Type = static_cast<WaveType>(osc1Type);
    preset >> m_osc1Gain;
    preset >> m_osc1NoteOffset;
    
    // OSC 2
    preset >> m_isOsc2Active;
    int osc2Type;
    preset >> osc2Type;
    m_osc2Type = static_cast<WaveType>(osc2Type);
    preset >> m_osc2Gain;
    preset >> m_osc2NoteOffset;
    
    // OSC 3
    preset >> m_isOsc3Active;
    int osc3Type;
    preset >> osc3Type;
    m_osc3Type = static_cast<WaveType>(osc3Type);
    preset >> m_osc3Gain;
    preset >> m_osc3NoteOffset;
    
    // LFO
    preset >> m_lfoFrequency;
    preset >> m_isLfoActive;
    
    preset.close();
}

void Synth::getAllPresets()
{
    if (!m_presets.empty())
        m_presets = {};

    for (const auto& file : std::filesystem::directory_iterator("./presets")) {
        std::string p = file.path().u8string();
        if (p.find(".preset") != std::string::npos) {
            p.erase(0, 10);
            m_presets.push_back(p.substr(0, p.find(".")));
        }
    }
}
