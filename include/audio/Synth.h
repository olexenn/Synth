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
    
    const std::array<Voice*, NumberOfVoices>& getVoices(); //debug
    
    
    static AdsrParams m_adsrParams;
    
    static Filter m_filter;
    
    static std::array<bool, 3> m_isOscActive;
    static std::array<WaveType, 3> m_oscType;
    static std::array<float, 3> m_oscGain;
    static std::array<int, 3> m_oscNoteOffset;
    
    static float m_lfoFrequency;
    static bool m_isLfoActive;

    static bool m_isChorusActive;
    static float m_chorusFrequency;
    static int m_chorusDepth;
    
    static std::vector<std::string> m_presets;
    
    static void getAllPresets();
    static void savePreset(std::string name);
    static void loadPreset(std::string name);
    static std::string getCurrentPath();
    static int m_counter;//debug
private:
    int m_polyphonyCounter = 0; //debug
    
    std::array<Voice*, NumberOfVoices> m_voices;
    
    Voice* findFreeVoice(int key);
    
    
};

#endif /* Synth_h */
