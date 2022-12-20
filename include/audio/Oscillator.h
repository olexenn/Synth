//
//  Oscillator.h
//  synth
//
//  Created by Алексей Дудник on 29.10.2022.
//

#ifndef Oscillator_h
#define Oscillator_h

enum WaveType
{
    SINE,
    SQUARE,
    TRIANGLE,
    SAW,
    NOISE
};

class Oscillator
{
public:
    Oscillator(WaveType *type, bool *isActive, int *noteOffset, float *gain);
    double getSample(int key, double time, double lfoFrequency);
    void draw();
    
private:
    WaveType *m_type;
    float *m_gain;
    bool *m_isActive;
    int *m_noteOffset;
    
    double calculateFrequency(int key);
};

#endif /* Oscillator_h */
