//
//  Audio.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include <iostream>
#include <cmath>

#include "Audio.h"

const int g_kNumSeconds = 5;
const int g_kSampleRate = 44100;
const int g_kFramesPerBuffer = 64;
const float g_kTimeStep = 1.0 / g_kSampleRate;

Audio::Audio() : m_stream(0), m_phase(0), m_fFrequency(0.0f), m_fTime(0.0f)
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cout << "ERROR::PORTAUDIO::Can't init portAudio" << std::endl;
        exit(1);
    }
}

Audio::~Audio()
{
    stop();
    close();
    Pa_Terminate();
}

bool Audio::open(PaDeviceIndex index)
{
    PaStreamParameters outputParameters;
    
    outputParameters.device = index;
    if (outputParameters.device == paNoDevice) return false;
    
    const PaDeviceInfo *pInfo = Pa_GetDeviceInfo(outputParameters.device);
    if (pInfo != 0)
        std::cout << "Output Device Name: " << pInfo->name << "\r";
    
    outputParameters.channelCount = 1; // mono
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    PaError err = Pa_OpenStream(&m_stream, nullptr, &outputParameters, g_kSampleRate, paFramesPerBufferUnspecified, paClipOff, &Audio::paCallback, this);
    
    if (err != paNoError)
        return false;
    
    err = Pa_SetStreamFinishedCallback(m_stream, &Audio::paStreamFinished);
    
    if (err != paNoError) {
        Pa_CloseStream(m_stream);
        m_stream = 0;
        return false;
    }
    
    return true;
}

bool Audio::close()
{
    if (m_stream == 0)
        return false;
    
    PaError err = Pa_CloseStream(m_stream);
    m_stream = 0;
    
    return (err == paNoError);
}

bool Audio::start()
{
    if (m_stream == 0)
        return false;
    
    
    PaError err = Pa_StartStream(m_stream);
    
    
    return (err == paNoError);
}

bool Audio::stop()
{
    if (m_stream == 0) return false;
    
    PaError err = Pa_StopStream(m_stream);
    
    return (err == paNoError);
}

void Audio::noteOn(int key)
{
    calculateFrequency(key);
    m_adsr.noteOn(m_fTime);
}

void Audio::noteOff()
{
    m_fFrequency = 0.0f;
    m_adsr.noteOff(m_fTime);
}

float Audio::getTriggerOn()
{
    return m_adsr.getTriggerOnTime();
}

float Audio::getTriggerOff()
{
    return m_adsr.getTriggerOffTime();
}

int Audio::paCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    float *out = static_cast<float*>(outputBuffer);
    
    (void)timeInfo;
    (void)inputBuffer;
    (void)statusFlags;
    
    for (int i = 0; i < framesPerBuffer; i++) {
//        float fOutput = std::sinf(m_fFrequency * 2.0 * M_PI * m_fTime);
        float fAmplitude = m_adsr.getAmplitude(m_fTime);
        
        float fOutput = fAmplitude * std::sinf(m_fFrequency * 2.0 * M_PI * m_fTime);
        
        *out++ = 0.5 * fOutput;
        m_fTime += g_kTimeStep;
    }
    
    return paContinue;
}

void Audio::paStreamFinishedMethod()
{
    std::cout << "Stream Completed" << std::endl;
}

// https://pages.mtu.edu/~suits/NoteFreqCalcs.html
void Audio::calculateFrequency(int key)
{
    float a = 1.05946309435;
    float hz = std::pow(a, static_cast<float>(key) - 48); // A3 note number
    m_fFrequency = 220.0 * hz; // A3 note frequency
}

float Audio::getTime()
{
    return m_fTime;
}

float Audio::getFrequency()
{
    return m_fFrequency;
}
