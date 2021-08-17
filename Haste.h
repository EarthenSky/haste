#pragma once

#include <vector>
#include "utility.h"

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"

#include "HasteController.h"
#include "IWaveBlock.h"
#include "IGrid.h"

using std::vector;
using std::unique_ptr;

using namespace iplug;
using namespace igraphics;

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
    vector<std::unique_ptr<IWaveBlock>> waveBlocks;
    int_pair_to_bool_map blockLocationMap; // TODO: switch to just using the map?

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
