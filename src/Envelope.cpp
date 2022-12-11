//
//  Envelope.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//
#include <imgui.h>
#include <imgui-knobs.h>

#include "Envelope.h"

// TODO: Fix Envelope to state machine
//Envelope::Envelope(float *attackTime, float* decayTime,
//                   float *sustainAmplitude, float* releaseTime)
//{
//    m_attackTime = attackTime;
//    m_decayTime = decayTime;
//    m_sustainAmplitude = sustainAmplitude;
//    m_releaseTime = releaseTime;
//}

void Envelope::noteOn(double time)
{
    m_actualSustainAmplitude = 0.0;
    m_triggerOnTime = time;
    m_noteOn = true;
}

void Envelope::noteOff(double time)
{
    m_triggerOffTime = time;
    m_noteOn = false;
}

double Envelope::getAmplitude(double time)
{
    double amplitude = 0.0;
    double lifeTime = 0.0;
    
    if (m_noteOn) {
        lifeTime = time - m_triggerOnTime;
        
        if (lifeTime <= m_attackTime)
            amplitude = (lifeTime / m_attackTime) * m_startAmplitude;
        
        else if (lifeTime > m_attackTime && lifeTime <= (m_attackTime + m_decayTime))
            amplitude = ((lifeTime - m_attackTime) / m_decayTime) * (m_sustainAmplitude - m_startAmplitude) + m_startAmplitude;
        
        else if (lifeTime > (m_attackTime + m_decayTime))
            amplitude = m_sustainAmplitude;
        
        m_actualSustainAmplitude = amplitude;
    } else {
        lifeTime = time - m_triggerOffTime;
        
        amplitude = ((lifeTime / m_releaseTime) * (0.0 - m_actualSustainAmplitude) + m_actualSustainAmplitude);
    }
    
    if (amplitude <= 0.0001)
        amplitude = 0.0;
    
    m_currentAmplitude = amplitude;
    
    return amplitude;
}

void Envelope::reset()
{
    m_noteOn = false;
    m_currentAmplitude = 0.0;
    m_triggerOnTime = 0.0;
    m_triggerOffTime = 0.0;
}

bool Envelope::isNoteOff()
{
    return !m_noteOn;
}

double Envelope::getCurrentAmplitude()
{
    return m_currentAmplitude;
}
