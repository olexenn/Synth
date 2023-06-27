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
#include "Synth.h"

// low-level audio staff
class Audio {
public:
    Audio();
    
    ~Audio();
    
    int getCounter(); //debug
    
    bool open(PaDeviceIndex index);
    
    bool close();
    
    bool start();
    
    bool stop();
    
    float getTime(); //debug
    float getLeftSample();
    float getRightSample();
    
    void noteOn(int key);
    
    void noteOff(int key);
    
    const std::array<Voice*, Synth::NumberOfVoices>& getVoices(); //debug
    
    static float m_panningValue;
    static float m_gain;
private:
    PaStream *m_stream;
    Synth m_synth;
    double m_time;
    float m_leftSample;
    float m_rightSample;
    
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
};

#endif /* Audio_h */
