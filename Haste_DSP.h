#pragma once

#include "MidiSynth.h"
#include "Oscillator.h"
#include "ADSREnvelope.h"
#include "Smoothers.h"

using namespace iplug;

enum EModulations {
    kModGainSmoother = 0,
    kModSustainSmoother,
    kNumModulations,
};

// T is our datatype.
template<typename T>
class MainVoice : public SynthVoice {
public:
    FastSinOscillator<T> mOSC;
    ADSREnvelope<T> mAMPEnv;

private:
    WDL_TypedBuf<float> mTimbreBuffer;
    uint32_t mRandSeed = 0; // noise generator for test

public:
    MainVoice() : mAMPEnv("gain", [&]() { mOSC.Reset(); }) // capture ok on RT thread?
    {
       //  DBGMSG("new Voice: %i control inputs.\n", static_cast<int>(mInputs.size()));
    }

    // True if the voice is currently producing sound.
    bool GetBusy() const override {
        return mAMPEnv.GetBusy();
    }

    // This manages how to react to notes starting & ending.
    void Trigger(double level, bool isRetrigger) override {
        mOSC.Reset();

        if (isRetrigger)
            mAMPEnv.Retrigger(level);
        else
            mAMPEnv.Start(level);
    }

    void Release() override {
        mAMPEnv.Release();
    }

    // This function is the main signal generation computation.
    void ProcessSamplesAccumulating(T** inputs, T** outputs, int nInputs, int nOutputs, int startIdx, int nFrames) override {
        // inputs to the synthesizer can just fetch a value every block, like this:
        double pitch = mInputs[kVoiceControlPitch].endValue;

        // or write the entire control ramp to a buffer, like this, to get sample-accurate ramps:
        mInputs[kVoiceControlTimbre].Write(mTimbreBuffer.Get(), startIdx, nFrames);

        // convert from "1v/oct" pitch space to frequency in Hertz
        double A4 = 440.0;
        double osc1Freq = A4 * pow(2.0, pitch); // +inputs[kModLFO][0]);

        // make sound output 
        for (auto i = startIdx; i < startIdx + nFrames; i++) {
            float noise = mTimbreBuffer.Get()[i] * Rand();

            auto wave = mOSC.Process(osc1Freq) + noise;
            auto env = mAMPEnv.Process(inputs[kModSustainSmoother][i]);

            // for each output channel
            outputs[0][i] += wave * env * mGain;
            outputs[1][i] = outputs[0][i];
        }
    }

    // Block size is how large each processing block is.
    // You may want to preallocate arrays that are the proper size to store block info.
    void SetSampleRateAndBlockSize(double sampleRate, int blockSize) override {
        mOSC.SetSampleRate(sampleRate);
        mAMPEnv.SetSampleRate(sampleRate);

        mTimbreBuffer.Resize(blockSize);
    }

    void SetProgramNumber(int pgm) override {
        //TODO:
    }

    // this is called by the VoiceAllocator to set generic control values.
    void SetControl(int controlNumber, float value) override {
        //TODO:
    }

private:

    // return single-precision floating point number on [-1, 1]
    float Rand()
    {
        // Yo, this actually kinda funky math!
        mRandSeed = mRandSeed * 0x0019660D + 0x3C6EF35F;
        uint32_t temp = ((mRandSeed >> 9) & 0x007FFFFF) | 0x3F800000;
        return (*reinterpret_cast<float*>(&temp)) * 2.f - 3.f;
    }
};

// DSP stands for Digital Signal Processor!
template<typename T>
class HasteDSP {
public:
    MidiSynth mSynth{ VoiceAllocator::kPolyModePoly, MidiSynth::kDefaultBlockSize };
    WDL_TypedBuf<T> mModulationsData; // Sample data for global modulations (e.g. smoothed sustain)
    WDL_PtrList<T> mModulations; // Ptrlist for global modulations
    LogParamSmooth<T, kNumModulations> mParamSmoother;
    T mParamsToSmooth[kNumModulations];

public:
    HasteDSP(int nVoices) {
        for (auto i = 0; i < nVoices; i++) {
            mSynth.AddVoice(new MainVoice<T>(), 0); // add a voice to Zone 0.
        } 

        // some MidiSynth API examples:
        // mSynth.SetKeyToPitchFn([](int k){return (k - 69.)/24.;}); // quarter-tone scale
        // mSynth.SetNoteGlideTime(0.5); // portamento
    }

    // block size is defined by the DAW & represents how much time is rendered at once.
    void ProcessBlock (
        T** inputs, T** outputs,
        int nOutputs, int nFrames,
        double qnPos = 0.0,
        bool transportIsRunning = false,
        double tempo = 120.0
    ) {
        // clear outputs
        for (auto i = 0; i < nOutputs; i++) {
            memset(outputs[i], 0, nFrames * sizeof(T));
        }
    
        mParamSmoother.ProcessBlock(mParamsToSmooth, mModulations.GetList(), nFrames); // TODO: I don't really get smoothing quite yet... -> maybe I do? -> To reduce popping?
        mSynth.ProcessBlock(mModulations.GetList(), outputs, 0, nOutputs, nFrames);
    
        for (int s = 0; s < nFrames; s++) {
            auto smoothedGain = mModulations.GetList()[kModGainSmoother][s];

            // both channels already have data, just apply our gain mod to it now.
            outputs[0][s] *= smoothedGain;
            outputs[1][s] *= smoothedGain;
        }
    }

    void Reset(double sampleRate, int blockSize) {
        mSynth.SetSampleRateAndBlockSize(sampleRate, blockSize);
        mSynth.Reset();

        mModulationsData.Resize(blockSize * kNumModulations);
        mModulations.Empty();
    
        for(int i = 0; i < kNumModulations; i++) {
            // yucky c style code (pointer arithematic) -> we can do better! 
            mModulations.Add(mModulationsData.Get() + (blockSize * i));
        }
    }

    void ProcessMidiMsg(const IMidiMsg& msg) {
        mSynth.AddMidiMsgToQueue(msg);
    }

    void SetParam(int paramIdx, double value) {
        using EEnvStage = ADSREnvelope<T>::EStage;
    
        switch (paramIdx) {
            case kParamGain:
                mParamsToSmooth[kModGainSmoother] = (T) value / 100.0;
                break;
            case kParamSustain:
                mParamsToSmooth[kModSustainSmoother] = (T) value / 100.0;
                break;
            case kParamAttack:
            case kParamDecay:
            case kParamRelease:
            {
                EEnvStage stage = static_cast<EEnvStage>(EEnvStage::kAttack + (paramIdx - kParamAttack));
                mSynth.ForEachVoice([stage, value](SynthVoice& voice) {
                    dynamic_cast<MainVoice<T>&>(voice).mAMPEnv.SetStageTime(stage, value);
                });
                break;
            }
            default:
                break;
        }
    }
};
