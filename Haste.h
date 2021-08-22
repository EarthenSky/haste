#pragma once

#include "common.h"

#include "IControls.h"

#include "IWaveBlock.h"
#include "IGrid.h"
#include "ILine.h"

#include "HasteController.h"

const int kNumPresets = 1;

enum EParams {
    kParamGain = 0,
    kParamAttack,
    kParamDecay,
    kParamSustain,
    kParamRelease,
    kNumParams
};

#if IPLUG_DSP
// will use EParams in Haste_DSP.h
#include "Haste_DSP.h"
#endif

enum EControlTags {
    kCtrlTagMeter = 0,
    kCtrlTagScope,
    kCtrlTagRTText,
    kCtrlTagKeyboard,
    kNumCtrlTags
};

class Haste final : public Plugin {
public:
    HasteController controller;
    unordered_map<Point2, IWaveBlock*, point2_hash> blockLocationMap;
    unordered_map<Vector2, ILine*, vector2_hash> connectionMap;
    // TODO: make a AST for the synths -> in order to optimize we're pretty much gonna be a compiler.

public:
    Haste(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
private:
    HasteDSP<sample> mDSP{ 16 };
    IPeakSender<2> mMeterSender;

public:
    void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
    void ProcessMidiMsg(const IMidiMsg& msg) override;
    void OnReset() override;
    void OnParamChange(int paramIdx) override;
    void OnIdle() override;
    bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;
#endif
};
