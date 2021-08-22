#pragma once

#include "HasteController.h"
#include "nanovg.h"

// -----------------------------
// Public

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
    if (mod.S && mod.R) {
        controller_.RemoveWaveBlock(myLocation);
    } else if (mod.L) {
        // TODO: something here?
    }
}

void IWaveBlock::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) {
    if (mod.L) {
        // TODO: change these two into a function so it's cleaner.
        transformedX += dX;
        transformedY += dY;
        SetDirty(false);
    } else if (mod.R) {
        // Pull a connection.
        LineTo(x + dX, y + dY);
    }
    dragging = true;
}

void IWaveBlock::OnMouseUp(float x, float y, const IMouseMod& mod) {
    if (mod.LUp) {
        auto targetIndex = CurrentLocation();
        if (!controller_.IsWaveBlockAt(targetIndex)) {
            controller_.MoveWaveBlock(myLocation, targetIndex);
            myLocation = targetIndex;
        }

        IRECT newRect = GetRECTAt(myLocation);
        transformedX = newRect.L - GetRECT().L;
        transformedY = newRect.T - GetRECT().T;
        SetDirty(false);
    } else if (mod.RUp && dragging) {
        auto targetLocation = Vector2(x, y);
        auto targetIndex = LocationToIndex(targetLocation);
        if (controller_.IsWaveBlockAt(targetIndex) && targetIndex != myLocation) {
            // Success, we found a block to attach to!
            Vector2 startLoc = IndexToLocation(myLocation, GetRECT().W(), GetRECT().H());
            Vector2 endLoc = IndexToLocation(targetIndex, GetRECT().W(), GetRECT().H());
            auto waveBlock = controller_.GetWaveBlock(targetIndex);
            CreateConnection(waveBlock, startLoc, endLoc);
            AddIngoingConnection(waveBlock);
        } else {
            EndLine();
        }
    }    
    dragging = false;
}

// -----------------------------
// Private

void IWaveBlock::LineTo(float targetX, float targetY) {
    // convert from index coordinates to world coordinates
    auto blockMiddle = IndexToLocation(myLocation, GetRECT().W(), GetRECT().H());
    auto start = blockMiddle;
    auto end = Vector2(targetX, targetY);
    if (controller_.IsConnectionAt(start)) {
        controller_.UpdateConnection(start, end);
    } else {
        outgoingLine = std::optional<ILine*>{ new ILine(start, end) };
        controller_.AddConnection(outgoingLine.value());
    }
}

void IWaveBlock::EndLine() {
    controller_.RemoveConnection(outgoingLine.value()->GetStart());
    outgoingLine = std::nullopt;
}

void IWaveBlock::CreateConnection(IWaveBlock* target, Vector2 start, Vector2 end) {
    outgoingConnection = std::optional<IWaveBlock*>{ target };
    outgoingLine = std::optional<ILine*>{ new ILine(start, end) };
    controller_.AddConnection(outgoingLine.value());
}

void IWaveBlock::DestroyConnection() {
    if (outgoingLine.has_value()) {
        controller_.RemoveConnection(outgoingLine.value()->GetStart());
        outgoingConnection = std::nullopt;
        outgoingLine = std::nullopt;
    }
}