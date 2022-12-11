//
//  Envelope.h
//  sandbox
//
//  Created by Алексей Дудник on 18.10.2022.
//

#ifndef Envelope_h
#define Envelope_h

enum AdsrMachine
{
    ATTACK,
    SUSTAIN,
    DECAY,
    RELEASE
};

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
//    Envelope(float *attackTime, float *decayTime,
//             float *sustainAmplitude, float *releaseTime);
    Envelope() = default;
    
    void noteOn(double time);
    
    void noteOff(double time);
    
    double getAmplitude(double time);
    
    void reset();
    
    bool isNoteOff();
    
    double getCurrentAmplitude();
    
    float& getAttack() { return m_attackTime; };
    float& getDecay() { return m_decayTime; };
    float& getSustain() { return m_sustainAmplitude; };
    float& getRelease() { return m_releaseTime; };
    
private:
    double m_startAmplitude = 1.0;
    double m_triggerOnTime = 0.0;
    double m_triggerOffTime = 0.0;
    double m_actualSustainAmplitude = 0.0;
    double m_currentAmplitude = 0.0;
    
    float m_attackTime = 0.1f;
    float m_decayTime = 0.01f;
    float m_sustainAmplitude = 0.8f;
    float m_releaseTime = 0.2f;
    
    bool m_noteOn = false;
    
    AdsrMachine m_state = ATTACK;
};

#endif /* Envelope_h */
