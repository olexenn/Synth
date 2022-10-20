//
//  Envelope.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Envelope_h
#define Envelope_h

// ADSR Envelope
// TODO: Better parameters manipulation
class Envelope {
public:
    Envelope();
    
    void noteOn(float time);
    
    void noteOff(float time);
    
    float getAmplitude(float time);
    
    void draw();
    
    void reset();
    
    bool isNoteOff();
    
    float getCurrentAmplitude();
    
private:
    float m_startAmplitude;
    float m_triggerOnTime;
    float m_triggerOffTime;
    float m_attackTime;
    float m_decayTime;
    float m_sustainAmplitude;
    float m_releaseTime;
    float m_actualSustainAmplitude;
    float m_currentAmplitude;
    
    bool m_noteOn;
};

#endif /* Envelope_h */
