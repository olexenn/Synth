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

#include "audio/Synth.h"

int Synth::m_counter = 0;
AdsrParams Synth::m_adsrParams = { 0.1, 0.1, 0.8, 0.2 };
std::array<bool, 3> Synth::m_isOscActive = { false, false, false };
std::array<WaveType, 3> Synth::m_oscType = { SINE, SINE, SINE };
std::array<float, 3> Synth::m_oscGain = { 0.5, 0.5, 0.5 };
std::array<int, 3> Synth::m_oscNoteOffset = { 0, 0, 0};
float Synth::m_lfoFrequency = 5.0;
bool Synth::m_isLfoActive = false;
std::vector<std::string> Synth::m_presets;
Filter Synth::m_filter;

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

    std::string binaryPath = getCurrentPath();
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
        return resultPath.substr(0, resultPath.find("dudo-synth"));
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
        return resultPath.substr(0, resultPath.find("dudo-synth"));
    }
    else {
        std::cout << "ERROR: Can't find path for executable\n";
        exit(1);
    }
#endif
}
