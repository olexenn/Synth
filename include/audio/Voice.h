//
//  Voice.h
//  synth
//
//  Created by Алексей Дудник on 20.10.2022.
//

#ifndef Voice_h
#define Voice_h

#include <array>

#include "Envelope.h"
#include "Oscillator.h"
#include "Filter.h"
#include "Lfo.h"

// Single note sound
class Voice
{
public:
    Voice(Envelope *pEnv, Oscillator *osc1, Oscillator *osc2,
          Oscillator *osc3, Lfo *lfo);
    
    double getSample(double time);
    
    bool isActive();
    
    void noteOn(int key, double time);
    void noteOff(double time);
    
    int getKey();
    
private:
    int m_key = 0;
    double m_timeOn = 0.0;
    double m_timeOff = 0.0;
    bool m_isActive = false;
    Envelope *m_adsr;
    std::array<Oscillator*, 3> m_oscillators;
    Lfo *m_lfo;
};

#endif /* Voice_h */
