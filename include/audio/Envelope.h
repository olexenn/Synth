//
//  Envelope.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Envelope_h
#define Envelope_h

struct AdsrParams
{
    float attackTime;
    float decayTime;
    float sustainAmplitude;
    float releaseTime;
};

// ADSR Envelope
class Envelope {
public:
    Envelope(float *attackTime, float *decayTime,
             float *sustainAmplitude, float *releaseTime);
    
    void noteOn(double time);
    
    void noteOff(double time);
    
    double getAmplitude(double time);
    
    void reset();
    
    bool isNoteOff();
    
    double getCurrentAmplitude();
    
private:
    double m_startAmplitude = 1.0;
    double m_triggerOnTime = 0.0;
    double m_triggerOffTime = 0.0;
    double m_actualSustainAmplitude = 0.0;
    double m_currentAmplitude = 0.0;
    
    float *m_attackTime;
    float *m_decayTime;
    float *m_sustainAmplitude;
    float *m_releaseTime;
    
    bool m_isNoteOn = false;
};

#endif /* Envelope_h */
