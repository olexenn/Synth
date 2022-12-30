#include <iostream>
#include <gtest/gtest.h>

#include "../include/audio/Envelope.h"
#include "../include/audio/Oscillator.h"

// Envelope Tests
class EnvelopeTest : public testing::Test
{
protected:
    void SetUp() override
    {
        m_env = new Envelope(&m_attack, &m_decay, &m_sustain, &m_release);
    }
    
    float m_attack = 0.1f;
    float m_decay = 0.1f;
    float m_sustain = 0.8f;
    float m_release = 0.2f;
    Envelope *m_env;
};

TEST_F(EnvelopeTest, AmplitudeOnSilent)
{
    EXPECT_TRUE(m_env->isNoteOff());
    EXPECT_EQ(0, m_env->getAmplitude(0));
}

TEST_F(EnvelopeTest, AmplitudeOnAttack)
{
    m_env->noteOn(140.0);
    EXPECT_FALSE(m_env->isNoteOff());
    EXPECT_LE(0, m_env->getAmplitude(140.1));
    EXPECT_GE(1, m_env->getAmplitude(140.1));
}

TEST_F(EnvelopeTest, AmplitudeOnDecay)
{
    m_env->noteOn(140.0);
    EXPECT_FALSE(m_env->isNoteOff());
    EXPECT_LE(m_sustain, m_env->getAmplitude(140.1));
    EXPECT_NEAR((double)m_sustain, m_env->getAmplitude(140.2), 0.0001);
}

TEST_F(EnvelopeTest, AmplitudeOnSustain)
{
    m_env->noteOn(140.0);
    EXPECT_FALSE(m_env->isNoteOff());
    EXPECT_EQ(m_sustain, m_env->getAmplitude(140.4));
}

TEST_F(EnvelopeTest, AmplitudeOnRelease)
{
    m_env->noteOn(140.0);
    EXPECT_FALSE(m_env->isNoteOff());
    m_env->noteOff(150.0);
    EXPECT_TRUE(m_env->isNoteOff());
    EXPECT_GE(m_sustain, m_env->getAmplitude(150.1));
    EXPECT_EQ(0, m_env->getAmplitude(150.3));
}

// Oscillator Test
class OscillatorTest : public testing::Test
{
protected:
    void SetUp() override
    {
        m_osc = new Oscillator(&m_type, &m_isActive, &m_noteOffset, &m_gain);
    }
    
    WaveType m_type = SINE;
    bool m_isActive = true;
    int m_noteOffset = 0;
    float m_gain = 1.0;
    Oscillator *m_osc;
};

TEST_F(OscillatorTest, Sine)
{
    EXPECT_EQ(0, m_osc->getSample(48, 0, 0.0));
    EXPECT_GE(1, m_osc->getSample(48, 1, 0.3));
}

TEST_F(OscillatorTest, Square)
{
    m_type = SQUARE;
    EXPECT_EQ(-0.8, m_osc->getSample(48, 0, 0.0));
    EXPECT_EQ(0.8, m_osc->getSample(48, 0.02, 0.0));
}

TEST_F(OscillatorTest, Saw)
{
    m_type = SAW;
    EXPECT_EQ(0, m_osc->getSample(48, 0, 0.0));
    EXPECT_GE(1, m_osc->getSample(48, 0, 0.3));
}

TEST_F(OscillatorTest, Triangle)
{
    m_type = TRIANGLE;
    EXPECT_EQ(0, m_osc->getSample(48, 0, 0.0));
    EXPECT_GE(1, m_osc->getSample(48, 0, 0.3));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cin.get();
    return result;
}
