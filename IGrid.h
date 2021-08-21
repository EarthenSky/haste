#pragma once

#include <utility>
#include <vector>
#include "utility.h"

#include "HasteController.h"

#include "IControl.h"
#include "IWaveBlock.h"

using namespace iplug;
using namespace igraphics;

class IGrid : public IControl {
private:
    enum MouseStatus {
        Over,
        Clicked,
        Off
    };

    // Todo: Make these constants
    // 100.0, 37.5, 12.5, 20
    float lineGap_, offsetX_, offsetY_, mainPadding_;
    HasteController& controller_;

    std::pair<int, int> targetLoc;
    MouseStatus targetStatus;
  
public:
    IGrid(HasteController& controller, const IRECT& bounds, float lineGap=100.0,
        float offsetX=0.0, float offsetY=0.0, float mainPadding=0.0)
        : controller_(controller), IControl(bounds), lineGap_(lineGap),
          offsetX_(offsetX), offsetY_(offsetY), mainPadding_(mainPadding) {}

    void Draw(IGraphics& g) override {
        if (targetStatus == MouseStatus::Clicked) {
            float x = targetLoc.first * lineGap_ + offsetX_ + mainPadding_;
            float y = targetLoc.second * lineGap_ + offsetY_ + mainPadding_;
            IRECT targetRect (x, y, x + lineGap_, y + lineGap_);
            g.FillRect(COLOR_MID_GRAY, targetRect);

            if (!controller_.IsWaveBlockAt(targetLoc)) {
                IRECT waveblockRect(x + 8, y + 8, x + lineGap_ - 8, y + lineGap_ - 8);
                //std::unique_ptr<IWaveBlock> waveBlock = std::make_unique<IWaveBlock>();
                controller_.AddWaveBlock(new IWaveBlock(controller_, waveblockRect), targetLoc);
            }
        } else if (targetStatus == MouseStatus::Over) {
            float x = targetLoc.first * lineGap_ + offsetX_ + mainPadding_;
            float y = targetLoc.second * lineGap_ + offsetY_ + mainPadding_;
            IRECT targetRect(x, y, x + lineGap_, y + lineGap_);
            g.FillRect(COLOR_BLACK_DROP_SHADOW, targetRect);
        }

        for (auto x = mRECT.L + offsetX_; x < mRECT.R + 0.001; x += lineGap_) {
            g.DrawDottedLine(COLOR_BLACK, x, mRECT.T, x, mRECT.B, nullptr, 1.0f, 2.0f); // dotted line?
        }

        for (auto y = mRECT.T + offsetY_; y < mRECT.B + 0.001; y += lineGap_) {
            g.DrawDottedLine(COLOR_BLACK, mRECT.L, y, mRECT.R, y, nullptr, 1.0f, 2.0f); // dotted line?
        }

        // TODO: Draw children? -> nope, just give me a reference to the data structures that hold them.
    }

    void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {
        targetLoc = std::make_pair(static_cast<int>((x - offsetX_ - mainPadding_) / lineGap_), static_cast<int>((y - offsetY_ - mainPadding_) / lineGap_));
        targetStatus = MouseStatus::Clicked;
        SetDirty(false); // false means "only visually dirty"
    }

    void OnMouseUp(float x, float y, const IMouseMod& mod) override {
        targetLoc = std::make_pair(static_cast<int>((x - offsetX_ - mainPadding_) / lineGap_), static_cast<int>((y - offsetY_ - mainPadding_) / lineGap_));
        targetStatus = MouseStatus::Off;
        SetDirty(false); // false means "only visually dirty"
    }

    void OnMouseOver(float x, float y, const IMouseMod& mod) override {
        targetLoc = std::make_pair(static_cast<int>((x - offsetX_ - mainPadding_) / lineGap_), static_cast<int>((y - offsetY_ - mainPadding_) / lineGap_));
        targetStatus = MouseStatus::Over;
        SetDirty(false); // false means "only visually dirty"
    }
};