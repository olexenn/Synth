//
//  Filter.h
//  synth
//
//  Created by Алексей Дудник on 30.11.2022.
//

#ifndef Filter_h
#define Filter_h

enum FilterType
{
    LOW_PASS,
    HIGH_PASS
};

class Filter
{
public:
    Filter();
    double getFilteredSample(double inputSample);
    
    void setLowCuttoff(float *lowCuttoff);
    void setHighCuttoff(float *highCuttoff);
    void setFilterType(FilterType *type);
    
private:
    double m_rad;
    double m_input0;
    double m_input1;
    double m_input2;
    double m_output1;
    double m_output2;
    double m_delayInput1;
    double m_delayInput2;
    double m_delayOutput1;
    double m_delayOutput2;
    double m_frequency;
    float *m_lowCuttoff;
    float *m_highCuttoff;
    
    FilterType *m_filterType;
    
    void calculateLowCoefficients();
    void calculateHighCoefficients();
};

#endif /* Filter_h */
