//
//  Audio.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include <iostream>

#include "audio/Audio.h"

constexpr int g_kSampleRate = 44100;
constexpr double g_kTimeStep = 1.0 / static_cast<float>(g_kSampleRate);

float Audio::m_panningValue = 1.0;
float Audio::m_gain = 0.5;

Audio::Audio() : m_stream(0), m_time(0.0)
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
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
    if (pInfo != 0) {
        auto hostApi = Pa_GetHostApiInfo(pInfo->hostApi);
        std::cout << "Host API: " << hostApi->name << std::endl;
    }
    
    outputParameters.channelCount = 2; // stereo
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    PaError err = Pa_OpenStream(&m_stream, nullptr, &outputParameters, g_kSampleRate, 64, paNoFlag, &Audio::paCallback, this);
    
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
    m_synth.noteOn(key, m_time);
}

void Audio::noteOff(int key)
{
    m_synth.noteOff(key, m_time);
}

int Audio::getCounter()
{
    return m_synth.getCounter();
}

int Audio::paCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    float *out = static_cast<float*>(outputBuffer);
    
    (void)timeInfo;
    (void)inputBuffer;
    (void)statusFlags;
    
    for (int i = 0; i < framesPerBuffer; i++) {
        m_sample = m_synth.getSample(m_time);
        *out++ = m_gain * (2 - m_panningValue) * m_sample; // left
        *out++ = m_gain * m_panningValue * m_sample; // right
        m_time += g_kTimeStep;
    }
    
    return paContinue;
}

void Audio::paStreamFinishedMethod()
{
//    std::cout << "Stream Completed" << std::endl;
}

float Audio::getTime()
{
    return m_time;
}

float Audio::getSample()
{
    return m_sample;
}

const std::array<Voice*, Synth::NumberOfVoices>& Audio::getVoices()
{
    return m_synth.getVoices();
}
