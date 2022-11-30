//
//  Audio.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include <iostream>
#include <cmath>
#include <limits>

#include <imgui.h>
#include <imgui-knobs.h>

#include "Audio.h"

const int g_kSampleRate = 44100;
const double g_kTimeStep = 1.0f / g_kSampleRate;

Audio::Audio() : m_stream(0), m_time(0.0), m_panningValue(1.0f), m_gain(0.5f)
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
    
    outputParameters.channelCount = 2; // mono
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
    synth.noteOn(key, m_time);
}

void Audio::noteOff(int key)
{
    synth.noteOff(key, m_time);
}

int Audio::getCounter()
{
    return synth.getCounter();
}

int Audio::paCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    float *out = static_cast<float*>(outputBuffer);
    
    (void)timeInfo;
    (void)inputBuffer;
    (void)statusFlags;
    
    for (int i = 0; i < framesPerBuffer; i++) {
        *out++ = m_gain * (2 - m_panningValue) * synth.getSample(m_time); // left
        *out++ = m_gain * m_panningValue * synth.getSample(m_time); // right
        m_time += g_kTimeStep;
    }
    
    return paContinue;
}

void Audio::paStreamFinishedMethod()
{
    std::cout << "Stream Completed" << std::endl;
}

float Audio::getTime()
{
    return m_time;
}

void Audio::draw()
{
    synth.draw();
    
    ImGui::Begin("Master");
    
    ImGuiKnobs::Knob("Panning", &m_panningValue, 0.0f, 2.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Volume", &m_gain, 0.0f, 2.0f);
    
    ImGui::End();
}

std::vector<float> Audio::getFrequencies()
{
    return synth.getFrequencies();
}

