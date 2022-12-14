#pragma once

#include "common.h"

#include "IControl.h"
#include "IWaveBlock.h"

using namespace iplug;
using namespace igraphics;

class HasteController;

class IOutputBlock : IWaveBlock {
    IOutputBlock(HasteController& controller, const IRECT& bounds, IControl* volume, IControl* pitch)
        : IWaveBlock(controller, bounds, volume, pitch) {}
}