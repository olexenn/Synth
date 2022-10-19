//
//  Audio.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Audio_h
#define Audio_h

#include <array>

#include <portaudio.h>

#include "Envelope.h"

const int g_kTableSize = 200;

// Init audio and synth staff
// TODO: Break it in 2 separate classes: Audio && Synth
class Audio {
    PaStream *m_stream;
    std::array<double, g_kTableSize> m_signal;
    int m_phase;
    float m_fFrequency;
    float m_fTime;
    std::string m_message;
    Envelope m_adsr;
    
    void calculateFrequency(int key);
    
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags);
    
    static int paCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData)
    {
        return ((Audio*)userData)->paCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
    }
    
    void paStreamFinishedMethod();
    
    static void paStreamFinished(void *userData)
    {
        return ((Audio*)userData)->paStreamFinishedMethod();
    }
    

    
public:
    Audio();
    
    ~Audio();
    
    bool open(PaDeviceIndex index);
    
    bool close();
    
    bool start();
    
    bool stop();
    
    float getTime();
    
    float getFrequency();
    
    void noteOn(int key);
    
    void noteOff();
    
    float getTriggerOn();
    float getTriggerOff();
    
};

#endif /* Audio_h */
