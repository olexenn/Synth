//
//  Lfo.cpp
//  synth
//
//  Created by Алексей Дудник on 11.12.2022.
//

#include "audio/Lfo.h"

Lfo::Lfo(float *frequency, bool *isActive)
{
    m_frequency = frequency;
    m_isActive = isActive;
}

float Lfo::getFrequency()
{
    return *m_frequency;
}

bool Lfo::isActive()
{
    return *m_isActive;
}
