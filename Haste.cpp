#include "Haste.h"
#include "IPlug_include_in_plug_src.h"
#include "LFO.h"

Haste::Haste(const InstanceInfo& info) : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
    GetParam(kParamGain)->InitDouble("Gain", 100.0, 0.0, 100.0, 0.01, "%");
    GetParam(kParamAttack)->InitDouble("Attack", 10.0, 1.0, 1000.0, 0.1, "ms", IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(3.));
    GetParam(kParamDecay)->InitDouble("Decay", 10.0, 1.0, 1000.0, 0.1, "ms", IParam::kFlagsNone, "ADSR", IParam::ShapePowCurve(3.));
    GetParam(kParamSustain)->InitDouble("Sustain", 50.0, 0.0, 100.0, 1, "%", IParam::kFlagsNone, "ADSR");
    GetParam(kParamRelease)->InitDouble("Release", 10.0, 2.0, 1000.0, 0.1, "ms", IParam::kFlagsNone, "ADSR");
    
#if IPLUG_EDITOR // http://bit.ly/2S64BDd
    mMakeGraphicsFunc = [&]() {
        return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
    };
  
    mLayoutFunc = [&](IGraphics* pGraphics) {
        pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
        pGraphics->AttachPanelBackground(COLOR_GREEN);
        pGraphics->EnableMouseOver(true);
        pGraphics->EnableMultiTouch(true);
    
    #ifdef OS_WEB
        pGraphics->AttachPopupMenuControl();
    #endif

        pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
        const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
        const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
        IRECT keyboardBounds = b.GetFromBottom(300);
        IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);
        pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
        pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5)), kCtrlTagBender);
        pGraphics->AttachControl(new IWheelControl(wheelsBounds.FracRectHorizontal(0.5, true), IMidiMsg::EControlChangeMsg::kModWheel));

        const IRECT controls = b.GetGridCell(1, 2, 2);
        pGraphics->AttachControl(new IVKnobControl(controls.GetGridCell(0, 2, 6).GetCentredInside(90), kParamGain, "Gain"));
        const IRECT sliders = controls.GetGridCell(2, 2, 6).Union(controls.GetGridCell(3, 2, 6)).Union(controls.GetGridCell(4, 2, 6));
        pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(0, 1, 4).GetMidHPadded(30.), kParamAttack, "Attack"));
        pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(1, 1, 4).GetMidHPadded(30.), kParamDecay, "Decay"));
        pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(2, 1, 4).GetMidHPadded(30.), kParamSustain, "Sustain"));
        pGraphics->AttachControl(new IVSliderControl(sliders.GetGridCell(3, 1, 4).GetMidHPadded(30.), kParamRelease, "Release"));
        pGraphics->AttachControl(new IVLEDMeterControl<2>(controls.GetFromRight(100).GetPadded(-30)), kCtrlTagMeter);
    
        pGraphics->AttachControl(new IVButtonControl(keyboardBounds.GetFromTRHC(200, 30).GetTranslated(0, -30), SplashClickActionFunc,
          "Show/Hide Keyboard", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE).WithLabelText({15.f, EVAlign::Middle})))->SetAnimationEndActionFunction(
          [pGraphics](IControl* pCaller) {
            static bool hide = false;
            pGraphics->GetControlWithTag(kCtrlTagKeyboard)->Hide(hide = !hide);
            pGraphics->Resize(PLUG_WIDTH, hide ? PLUG_HEIGHT / 2 : PLUG_HEIGHT, pGraphics->GetDrawScale());
        });
    
        pGraphics->SetQwertyMidiKeyHandlerFunc([pGraphics](const IMidiMsg& msg) {
            pGraphics->GetControlWithTag(kCtrlTagKeyboard)->As<IVKeyboardControl>()->SetNoteFromMidi(msg.NoteNumber(), msg.StatusMsg() == IMidiMsg::kNoteOn);
        });
    };
#endif
}

#if IPLUG_DSP
void Haste::ProcessBlock(sample** inputs, sample** outputs, int nFrames) {
    mDSP.ProcessBlock(nullptr, outputs, 2, nFrames, mTimeInfo.mPPQPos, mTimeInfo.mTransportIsRunning);
    mMeterSender.ProcessBlock(outputs, nFrames, kCtrlTagMeter);
}

void Haste::OnIdle() {
    mMeterSender.TransmitData(*this);
}

void Haste::OnReset() {
    mDSP.Reset(GetSampleRate(), GetBlockSize());
}

void Haste::ProcessMidiMsg(const IMidiMsg& msg) {
    switch (msg.StatusMsg())
    {
        case IMidiMsg::kNoteOn:
        case IMidiMsg::kNoteOff:
        case IMidiMsg::kPolyAftertouch:
        case IMidiMsg::kControlChange:
        case IMidiMsg::kProgramChange:
        case IMidiMsg::kChannelAftertouch:
        case IMidiMsg::kPitchWheel:
        {
            break;
        }
        default:
            return;
    }
 
    mDSP.ProcessMidiMsg(msg);
    SendMidiMsg(msg);
}

void Haste::OnParamChange(int paramIdx)
{
    mDSP.SetParam(paramIdx, GetParam(paramIdx)->Value());
}

bool Haste::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
    if(ctrlTag == kCtrlTagBender && msgTag == IWheelControl::kMessageTagSetPitchBendRange)
    {
        const int bendRange = *static_cast<const int*>(pData);
        mDSP.mSynth.SetPitchBendRange(bendRange);
    }
  
    return false;
}
#endif
