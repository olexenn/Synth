//
//  Envelope.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Envelope_h
#define Envelope_h

// ADSR Envelope
class Envelope {
public:
    Envelope() = default;
    
    void noteOn(float time);
    
    void noteOff(float time);
    
    float getAmplitude(float time);
    
    void reset();
    
    bool isNoteOff();
    
    float getCurrentAmplitude();
    
    float* m_attackTime;
    float* m_decayTime;
    float* m_sustainAmplitude;
    float* m_releaseTime;
private:
    float m_startAmplitude = 1.0f;
    float m_triggerOnTime = 0.0f;
    float m_triggerOffTime = 0.0f;
    float m_actualSustainAmplitude = 0.0f;
    float m_currentAmplitude = 0.0f;
    
    bool m_noteOn = false;
};

#endif /* Envelope_h */
