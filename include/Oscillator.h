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
    Oscillator() = default;
    double getSample(int key, double time);
    void draw();
    void setType(WaveType *type);
    void setGain(float *gain);
    void setActive(bool *isOscActive);
    void setLfoActivity(bool *isLfoActive);
    void setLfoFrequency(float *frequency);
    void setNoteOffset(int *offset);
    
private:
    WaveType *m_type;
    float *m_gain;
    bool *m_isActive;
    float *m_lfoFrequency;
    bool *m_isLfoActive;
    int *m_noteOffset;
    
    double calculateFrequency(int key);
};

#endif /* Oscillator_h */
