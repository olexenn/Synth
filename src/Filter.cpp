//
//  Filter.cpp
//  synth
//
//  Created by Алексей Дудник on 30.11.2022.
//

#include <cmath>

#include "Filter.h"

Filter::Filter() :
m_input0(0.0),
m_input1(0.0),
m_input2(0.0),
m_output1(0.0),
m_output2(0.0),
m_delayInput1(0.0),
m_delayInput2(0.0),
m_delayOutput1(0.0),
m_delayOutput2(0.0),
m_frequency(0.0)
{
//    float initialCuttoff = 20000.0f;
//    m_cuttoff = &initialCuttoff;
    m_rad = M_PI / 44100.0;
//    calculateCoefficients();
}

void Filter::calculateLowCoefficients()
{
    if (*m_lowCuttoff < 1)
        *m_lowCuttoff = 1;
    if (m_frequency < 0.5)
        m_frequency = 0.5;
    
    double c = 1 / std::tan(m_rad * *m_lowCuttoff);
    double c2 = c * c;
    double csqr2 = std::sqrt(2) * c;
    double oned = 1.0 / (c2 + csqr2 + 1.0);
    
    m_input0 = oned;
    m_input1 = oned + oned;
    m_input2 = oned;
    m_output1 = (2.0 * (1.0 - c2)) * oned;
    m_output2 = (c2 - csqr2 + 1.0) * oned;
}

void Filter::calculateHighCoefficients()
{
    double c = std::tan(m_rad * *m_highCuttoff);
    double c2 = c * c;
    double csqr2 = std::sqrt(2) * c;
    double oned = 1.0 / (1.0 + c2 + csqr2);
    
    m_input0 = oned;
    m_input1 = -(m_input0 + m_input0);
    m_input2 = m_input0;
    m_output1 = (2.0 * (c2 - 1.0)) * oned;
    m_output2 = (1.0 - csqr2 + c2) * oned;
}

double Filter::getFilteredSample(double inputSample)
{
    switch (*m_filterType) {
        case LOW_PASS:
            calculateLowCoefficients();
            break;
        case HIGH_PASS:
            calculateHighCoefficients();
            break;
        default:
            break;
    }
    double out = (m_input0 * inputSample) + (m_input1 * m_delayInput1) + (m_input2 * m_delayInput2) - (m_output1 * m_delayOutput1) - (m_output2 * m_delayOutput2);
    m_delayOutput2 = m_delayOutput1;
    m_delayOutput1 = out;
    m_delayInput2 = m_delayInput1;
    m_delayInput1 = inputSample;
    
    return out;
}

void Filter::setLowCuttoff(float *lowCuttoff)
{
    m_lowCuttoff = lowCuttoff;
}

void Filter::setHighCuttoff(float *highCuttoff)
{
    m_highCuttoff = highCuttoff;
}

void Filter::setFilterType(FilterType *type)
{
    m_filterType = type;
}