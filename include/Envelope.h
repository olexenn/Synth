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
    
    void noteOn(float fTime);
    
    void noteOff(float fTime);
    
    float getAmplitude(float fTime);
    
    float getTriggerOnTime();
    
    float getTriggerOffTime();
    
    static float m_fAttackTime;
    static float m_fDecayTime;
    static float m_fSustainAmplitude;
    static float m_fReleaseTime;
private:
    float m_fStartAmplitude;
    float m_fTriggerOnTime;
    float m_fTriggerOffTime;
    
    bool m_bNoteOn;
};

#endif /* Envelope_h */
