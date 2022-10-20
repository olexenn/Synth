//
//  Envelope.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//
#include <imgui.h>
#include <imgui-knobs.h>

#include "Envelope.h"

Envelope::Envelope() :
m_attackTime(0.1f),
m_decayTime(0.01f),
m_sustainAmplitude(0.8f),
m_releaseTime(0.2f),
m_startAmplitude(1.0f),
m_triggerOnTime(0.0f),
m_triggerOffTime(0.0f),
m_actualSustainAmplitude(0.0f),
m_currentAmplitude(0.0f),
m_noteOn(false)
{}

void Envelope::noteOn(float time)
{
    m_actualSustainAmplitude = 0.0f;
    m_triggerOnTime = time;
    m_noteOn = true;
}

void Envelope::noteOff(float fTime)
{
    m_triggerOffTime = fTime;
    m_noteOn = false;
}

float Envelope::getAmplitude(float time)
{
    float amplitude = 0.0f;
    float lifeTime = 0.0f;
    
    if (m_noteOn) {
        lifeTime = time - m_triggerOnTime;
        
        if (lifeTime <= m_attackTime)
            amplitude = (lifeTime / m_attackTime) * m_startAmplitude;
        
        if (lifeTime > m_attackTime && lifeTime <= (m_attackTime + m_decayTime))
            amplitude = ((lifeTime - m_attackTime) / m_decayTime) * (m_sustainAmplitude - m_startAmplitude) + m_startAmplitude;
        
        if (lifeTime > (m_attackTime + m_decayTime))
            amplitude = m_sustainAmplitude;
        
        m_actualSustainAmplitude = amplitude;
    } else {
        lifeTime = time - m_triggerOffTime;
        
        amplitude = ((lifeTime / m_releaseTime) * (0.0f - m_actualSustainAmplitude) + m_actualSustainAmplitude);
    }
    
    if (amplitude <= 0.0001f)
        amplitude = 0.0f;
    
    m_currentAmplitude = amplitude;
    
    return amplitude;
}

void Envelope::reset()
{
    m_noteOn = false;
    m_currentAmplitude = 0.0f;
    m_triggerOnTime = 0.0f;
    m_triggerOffTime = 0.0f;
}

bool Envelope::isNoteOff()
{
    return (!m_noteOn);
}

float Envelope::getCurrentAmplitude()
{
    return m_currentAmplitude;
}


void Envelope::draw()
{
    ImGui::Begin("ADSR");
    
    ImGuiKnobs::Knob("Attack", &m_attackTime, 0.00000001f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &m_decayTime, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &m_sustainAmplitude, 0.00000001f, 1.0f);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &m_releaseTime, 0.00000001f, 2.0f);
    
    ImGui::End();
}
