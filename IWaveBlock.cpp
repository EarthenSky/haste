#pragma once

#include "HasteController.h"
#include "nanovg.h"

// Do all movement in the draw function (so we can clean the old frame first)
void IWaveBlock::Draw(IGraphics& g) {
    SetTargetAndDrawRECTs(GetRECT().GetTranslated(transformedX, transformedY));

    // TODO: implement a better way to accomplish this.
    // override iplugin's clipping rectangle so we can draw our transformed rectangle.
    auto mVG = (NVGcontext*) g.GetDrawContext();
    nvgScissor(mVG, mRECT.L, mRECT.T, mRECT.W(), mRECT.H());

    g.FillRect(COLOR_GRAY, mRECT);

    transformedX = 0.0; 
    transformedY = 0.0;
}

void IWaveBlock::OnMouseDown(float x, float y, const IMouseMod& mod) {
    if (mod.R) {
        controller_.RemoveWaveBlock(myLocation);
    }
}

void IWaveBlock::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) {
    if (mod.L) {
        // TODO: change these two into a function so it's clearer.
        transformedX += dX;
        transformedY += dY;
        SetDirty(false);
    } else if (mod.R) {
        // TODO: pull a connection.
    }
}

void IWaveBlock::OnMouseUp(float x, float y, const IMouseMod& mod) {
    auto targetLocation = CurrentLocation();
    if (!controller_.IsWaveBlockAt(targetLocation)) {
        controller_.MoveWaveBlock(myLocation, targetLocation);
        myLocation = targetLocation;
    }

    IRECT newRect = GetRECTAt(myLocation);
    transformedX = newRect.L - GetRECT().L;
    transformedY = newRect.T - GetRECT().T;
    SetDirty(false);
}