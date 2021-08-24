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
        DestroyAllConnections();
        controller_.RemoveWaveBlock(myLocation);
    } else if (mod.L) {
        // TODO: something here?
    }
}

void IWaveBlock::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) {
    if (mod.L) {
        Move(dX, dY);
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
        Move(newRect.L - GetRECT().L, newRect.T - GetRECT().T);
    } else if (mod.RUp && dragging) {
        auto targetLocation = Vector2(x, y);
        auto targetIndex = LocationToIndex(targetLocation);
        if (controller_.IsWaveBlockAt(targetIndex) && targetIndex != myLocation) {
            // Success, we found a block to attach to! (We can't attach to ourselves)
            Vector2 endLoc = IndexToLocation(targetIndex, GetRECT().W(), GetRECT().H());
            auto waveBlock = controller_.GetWaveBlock(targetIndex);

            if (outgoingConnection.has_value()) {
                outgoingConnection.value()->RemoveIngoingConnection(this->uid);
            }

            // create connection
            outgoingConnection = std::optional<IWaveBlock*>{ waveBlock };
            waveBlock->AddIngoingConnection(this);
            LineTo(endLoc);
        } else {
            EndOutgoingConnection();
        }
    }    
    dragging = false;
}

void IWaveBlock::EndOutgoingConnection() {
    if(controller_.IsConnectionFrom(this->uid)) 
        controller_.RemoveConnection(this->uid);
    outgoingLine = std::nullopt;
    outgoingConnection = std::nullopt;
}

// -----------------------------
// Private

void IWaveBlock::LineTo(Vector2 target) {
    LineTo(target.x, target.y);
}

void IWaveBlock::LineTo(float targetX, float targetY) {
    // convert from index coordinates to world coordinates
    auto start = IndexToLocation(myLocation, GetRECT().W(), GetRECT().H()); // block middle
    auto end = Vector2(targetX, targetY);
    if (controller_.IsConnectionFrom(this->uid)) {
        controller_.UpdateConnection(this->uid, start, end);
    } else {
        outgoingLine = std::optional<ILine*>{ new ILine(start, end) };
        controller_.AddConnection(this->uid, outgoingLine.value());
    }
}

// TODO: is this function needed?
void IWaveBlock::DestroyAllConnections() {
    // destroy outgoing connection
    EndOutgoingConnection();

    // ask all incoming connections to end their outgoing lines
    for (auto ingoingConnection : ingoingConnections) {
        ingoingConnection->EndOutgoingConnection();
    }
}