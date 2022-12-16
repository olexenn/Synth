//
//  Synth.h
//  synth
//
//  Created by Алексей Дудник on 19.10.2022.
//

#ifndef Synth_h
#define Synth_h

#include <vector>
#include <array>
#include <string>
#include <memory>

#include "Envelope.h"
#include "Voice.h"

// Main sound machine
class Synth {
public:
    static const int NumberOfVoices = 20; // max polyphony is 20 notes
    Synth();
    
    double getSample(double time);
    
    void noteOn(int key, double time);
    void noteOff(int key, double time);
    
    int getCounter(); //debug
    
    void draw(ImGuiStyle& style);
    
    
    const std::array<Voice*, NumberOfVoices>& getVoices(); //debug
    
private:
    int m_polyphonyCounter = 0; //debug
    
    AdsrParams m_adsrParams;
    
    Filter m_filter;
    
    std::array<Voice*, NumberOfVoices> m_voices;
    
    std::array<bool, 3> m_isOscActive;
    std::array<WaveType, 3> m_oscType;
    std::array<float, 3> m_oscGain;
    std::array<int, 3> m_oscNoteOffset;
    
    float m_lfoFrequency;
    bool m_isLfoActive;
    
    std::vector<std::string> m_presets;
    void getAllPresets();
    void savePreset(std::string name);
    void loadPreset(std::string name);
    
    std::string getCurrentPath();
    
    Voice* findFreeVoice(int key);
    
    int m_counter = 0;//debug
    
};

#endif /* Synth_h */
