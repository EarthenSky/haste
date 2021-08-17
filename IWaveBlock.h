#pragma once

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class IWaveBlock : public IControl {
public:
    IWaveBlock(const IRECT& bounds) : IControl(bounds) {
        
    }

    void Draw(IGraphics& g) override {
        g.FillRect(COLOR_GRAY, mRECT);

        // TODO: do all the things we want here

    }
};