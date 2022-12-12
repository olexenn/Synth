//
//  Filter.h
//  synth
//
//  Created by Алексей Дудник on 30.11.2022.
//

#ifndef Filter_h
#define Filter_h

#include <cmath>

#ifndef M_PI
#define M_PI  (3.14159265358979323846)
#endif

enum FilterType
{
    LOW_PASS,
    HIGH_PASS
};

class Filter
{
public:
    Filter() = default;
    double getFilteredSample(double inputSample);
    
    void setFilterType(FilterType type);
    
    FilterType getFilterType() { return m_filterType; }
    bool isFilterOn() { return m_isActive; }
    float& getLowCuttoff() { return m_lowCuttoff; }
    float& getHighCuttoff() { return m_highCuttoff; }
    bool& getActivity() { return m_isActive; }
    
    
private:
    double m_rad = M_PI / 44100.0;
    double m_input0 = 0.0;
    double m_input1 = 0.0;
    double m_input2 = 0.0;
    double m_output1 = 0.0;
    double m_output2 = 0.0;
    double m_delayInput1 = 0.0;
    double m_delayInput2 = 0.0;
    double m_delayOutput1 = 0.0;
    double m_delayOutput2 = 0.0;
    double m_frequency = 0.0;
    
    bool m_isActive = false;
    float m_lowCuttoff = 20000.0f;
    float m_highCuttoff = 10.0f;
    FilterType m_filterType = LOW_PASS;
    
    void calculateLowCoefficients();
    void calculateHighCoefficients();
};

#endif /* Filter_h */
