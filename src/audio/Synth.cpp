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

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <imgui.h>
#include <imgui-knobs.h>

#include "audio/Synth.h"

Synth::Synth()
{
    
    getAllPresets();
    loadPreset(m_presets[0]);
    
    std::array<Oscillator*, 3> pOsc;
    for (int i = 0; i < 3; i++) {
        pOsc[i] = new Oscillator(&m_oscType[i], &m_isOscActive[i],
                                 &m_oscNoteOffset[i], &m_oscGain[i]);
    }
    
    auto pLfo = new Lfo(&m_lfoFrequency, &m_isLfoActive);
    
    
    for (int i = 0; i < NumberOfVoices; i++) {
        auto pEnv = new Envelope(&m_adsrParams.attackTime, &m_adsrParams.decayTime,
                             &m_adsrParams.sustainAmplitude, &m_adsrParams.releaseTime);
        auto voice = new Voice(pEnv, pOsc[0], pOsc[1], pOsc[2], pLfo);
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
    
    // OSCs
    for (int i = 0; i < 3; i++) {
        std::string name = "Oscillator " + std::to_string(i + 1);
        ImGui::Begin(name.c_str());
        ImGuiKnobs::Knob("Gain", &m_oscGain[i], 0.0f, 1.0f, 0.05f);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Offset", &m_oscNoteOffset[i], 0, 48, 12);
        ImGui::SameLine();
        ImGui::Checkbox("Active", &m_isOscActive[i]);
        if (ImGui::Selectable("Sine", m_oscType[i] == SINE)) m_oscType[i] = SINE;
        if (ImGui::Selectable("Square", m_oscType[i] == SQUARE)) m_oscType[i] = SQUARE;
        if (ImGui::Selectable("Saw", m_oscType[i] == SAW)) m_oscType[i] = SAW;
        if (ImGui::Selectable("Triangle", m_oscType[i] == TRIANGLE)) m_oscType[i] = TRIANGLE;
        if (ImGui::Selectable("Noise", m_oscType[i] == NOISE)) m_oscType[i] = NOISE;
        ImGui::End();
    }
    
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
    preset.open(getCurrentPath() + "presets/" + name);
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
    for (int i = 0; i < 3; i++) {
        preset << m_isOscActive[i] << std::endl;
        preset << m_oscType[i] << std::endl;
        preset << m_oscGain[i] << std::endl;
        preset << m_oscNoteOffset[i] << std::endl;
    }
    
    // LFO
    preset << m_lfoFrequency << std::endl;
    preset << m_isLfoActive << std::endl;
    
    preset.close();
}

void Synth::loadPreset(std::string name)
{
    std::string fullName = name + ".preset";
    std::ifstream preset;
    preset.open(getCurrentPath() + "presets/" + fullName);
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
    
    // OSCs
    for (int i = 0; i < 3; i++) {
        preset >> m_isOscActive[i];
        int type;
        preset >> type;
        m_oscType[i] = static_cast<WaveType>(type);
        preset >> m_oscGain[i];
        preset >> m_oscNoteOffset[i];
    }
    
    // LFO
    preset >> m_lfoFrequency;
    preset >> m_isLfoActive;
    
    preset.close();
}

void Synth::getAllPresets()
{
    if (!m_presets.empty())
        m_presets = {};

    for (const auto& file : std::filesystem::directory_iterator(getCurrentPath() + "presets")) {
        std::string p = file.path().u8string();
        std::string parent = file.path().parent_path().u8string();
        if (p.find(".preset") != std::string::npos) {
            p.erase(0, parent.length() + 1);
            m_presets.push_back(p.substr(0, p.find(".")));
        }
    }
}

std::string Synth::getCurrentPath()
{
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        std::string resultPath = path;
        return resultPath.substr(0, resultPath.find("synth"));
    }
    else {
        std::cout << "ERROR: Can't find path for executable\n";
        exit(1);
    }
#endif

#ifdef _WIN32
    char path[1024];
    uint32_t size = sizeof(path);
    if (GetModuleFileName(NULL, path, size) != 0) {
        std::string resultPath = path;
        return resultPath.substr(0, resultPath.find("synth"));
    }
    else {
        std::cout << "ERROR: Can't find path for executable\n";
        exit(1);
    }
#endif
}
