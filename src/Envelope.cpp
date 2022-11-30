//
//  Envelope.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//
#include <imgui.h>
#include <imgui-knobs.h>

#include "Envelope.h"

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
        
        if (lifeTime <= *m_attackTime)
            amplitude = (lifeTime / *m_attackTime) * m_startAmplitude;
        
        if (lifeTime > *m_attackTime && lifeTime <= (*m_attackTime + *m_decayTime))
            amplitude = ((lifeTime - *m_attackTime) / *m_decayTime) * (*m_sustainAmplitude - m_startAmplitude) + m_startAmplitude;
        
        if (lifeTime > (*m_attackTime + *m_decayTime))
            amplitude = *m_sustainAmplitude;
        
        m_actualSustainAmplitude = amplitude;
    } else {
        lifeTime = time - m_triggerOffTime;
        
        amplitude = ((lifeTime / *m_releaseTime) * (0.0f - m_actualSustainAmplitude) + m_actualSustainAmplitude);
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

void Envelope::setAttackTime(float *time)
{
    m_attackTime = time;
}

void Envelope::setDecayTime(float *time)
{
    m_decayTime = time;
}

void Envelope::setSustainAmplitude(float *amp)
{
    m_sustainAmplitude = amp;
}

void Envelope::setReleaseTime(float *time)
{
    m_releaseTime = time;
}
