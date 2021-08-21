#pragma once

#include "utility.h"

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class HasteController;

// TODO: set grid as a "peer"
class IWaveBlock : public IControl {
public:
    HasteController& controller_;

private:
    float lineGap_ = 100.0, offsetX_ = 37.5, offsetY_ = 12.5, mainPadding_ = 20.0;
    float transformedX = 0.0, transformedY = 0.0;
    int_pair myLocation;

public:
    IWaveBlock(HasteController& controller, const IRECT& bounds)
    : controller_(controller), IControl(bounds) {
        myLocation = CurrentLocation();
    }

    int_pair CurrentLocation() {
        const IRECT& bounds = GetRECT();
        return std::make_pair((int)((bounds.L + bounds.W()/2 - offsetX_ - mainPadding_) / lineGap_),
                              (int)((bounds.T + bounds.H()/2 - offsetY_ - mainPadding_) / lineGap_));
    }
    
    // TODO: make this a utility function
    IRECT GetRECTAt(int_pair location) {
        float x = (location.first) * lineGap_ + offsetX_ + mainPadding_;
        float y = (location.second) * lineGap_ + offsetY_ + mainPadding_;
        return IRECT(x + 8, y + 8, x + lineGap_ - 8, y + lineGap_ - 8);
    }

    void Draw(IGraphics& g) override;
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    void OnMouseUp(float x, float y, const IMouseMod& mod) override;
};