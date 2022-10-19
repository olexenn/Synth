//
//  Envelope.cpp
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#include "Envelope.h"

float Envelope::m_fAttackTime = 0.1f;
float Envelope::m_fDecayTime = 0.01f;
float Envelope::m_fSustainAmplitude = 0.8f;
float Envelope::m_fReleaseTime = 0.2f;

Envelope::Envelope() :
m_fStartAmplitude(1.0f),
m_fTriggerOnTime(0.0f),
m_fTriggerOffTime(0.0f),
m_bNoteOn(false)
{}

void Envelope::noteOn(float fTime)
{
    m_fTriggerOnTime = fTime;
    m_bNoteOn = true;
}

void Envelope::noteOff(float fTime)
{
    m_fTriggerOffTime = fTime;
    m_bNoteOn = false;
}

float Envelope::getAmplitude(float fTime)
{
    float fAmplitude = 0.0f;
    float fLifeTime = fTime - m_fTriggerOnTime;

    if (m_bNoteOn) {
        if (fLifeTime <= m_fAttackTime)
            fAmplitude = (fLifeTime / m_fAttackTime) * m_fStartAmplitude;

        if (fLifeTime > m_fAttackTime && fLifeTime <= (m_fAttackTime + m_fDecayTime))
            fAmplitude = ((fLifeTime - m_fAttackTime) / m_fDecayTime) * (m_fSustainAmplitude - m_fStartAmplitude) + m_fStartAmplitude;

        if (fLifeTime > (m_fAttackTime + m_fDecayTime))
            fAmplitude = m_fSustainAmplitude;
    } else {
        fAmplitude = ((fTime - m_fTriggerOffTime) / m_fReleaseTime) * (0.0f - m_fSustainAmplitude) + m_fSustainAmplitude;
    }
    
    if (fAmplitude <= 0.0001)
        fAmplitude = 0.0;
    
    return fAmplitude;
}

float Envelope::getTriggerOnTime()
{
    return m_fTriggerOnTime;
}

float Envelope::getTriggerOffTime()
{
    return m_fTriggerOffTime;
}
