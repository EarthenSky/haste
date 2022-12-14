//#pragma once

#include "HasteController.h"
#include "nanovg.h"

// TODO: how to override & call the past function (for dry)

override void IOutputBlock::Draw(IGraphics& g) {
    g.FillRect(COLOR_ORANGE, mRECT.GetScaled(1.1f));
    this->IWaveBlock::Draw(g); // This should call overriden function?
}