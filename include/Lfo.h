//
//  Lfo.h
//  synth
//
//  Created by Алексей Дудник on 10.12.2022.
//

#ifndef Lfo_h
#define Lfo_h

class Lfo
{
public:
    Lfo(float *frequency, bool *isActive);
    
    float getFrequency();
    bool isActive();

private:
    bool *m_isActive;
    float *m_frequency;
};

#endif /* Lfo_h */
