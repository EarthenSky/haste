#pragma once

#include "common.h"
#include "ILine.h"

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class HasteController;

// TODO: rewrite all the graphics stuff using nanovg without any of this control stuff.
class IWaveBlock : public IControl {
public:
    const int uid;
    HasteController& controller_;

private:
    float transformedX = 0.0, transformedY = 0.0;
    Point2 myLocation;

    bool dragging = false;

    // we're small enough that this vector will never get to be more than a few hundred connections
    vector<IWaveBlock*> ingoingConnections;
    std::optional<IWaveBlock*> outgoingConnection;
    std::optional<ILine*> outgoingLine;

    IControl* volume;
    IControl* pitch;
    
public:
    IWaveBlock(HasteController& controller, const IRECT& bounds, IControl* volume, IControl* pitch)
    : controller_(controller), IControl(bounds), uid(GetUid())
    {
        myLocation = CurrentLocation();
        outgoingLine = std::nullopt;

        this->volume = volume;
        this->pitch = pitch;
    }

    Point2 CurrentLocation() {
        const IRECT& bounds = GetRECT();
        return Point2((int)((bounds.L + bounds.W()/2 - offsetX_ - mainPadding_) / lineGap_),
                      (int)((bounds.T + bounds.H()/2 - offsetY_ - mainPadding_) / lineGap_));
    }

    void Move(float dX, float dY) {
        transformedX += dX;
        transformedY += dY;
        SetDirty(false);

        volume->SetRECT(volume->GetRECT().GetTranslated(dX, dY));
        pitch->SetRECT(pitch->GetRECT().GetTranslated(dX, dY));

        // Move outgoing connections
        if (outgoingLine.has_value())
            outgoingLine.value()->MoveStart(dX, dY);

        // Move ingoing conections
        for (auto ingoingConnection : ingoingConnections) {
            auto ingoingLine = ingoingConnection->GetLine();
            if (ingoingLine.has_value())
                ingoingLine.value()->MoveEnd(dX, dY);
        }
    }

    void Draw(IGraphics& g) override;
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    void OnMouseUp(float x, float y, const IMouseMod& mod) override;

    void AddIngoingConnection(IWaveBlock* waveBlock) {
        ingoingConnections.push_back(waveBlock);
    }

    void RemoveIngoingConnection(int uidToRemove) {
        auto CheckUid = [uidToRemove](IWaveBlock* in) { return in->uid == uidToRemove; };
        auto at = std::find_if(ingoingConnections.begin(), ingoingConnections.end(), CheckUid);
        ingoingConnections.erase(at);
    }

    void EndOutgoingConnection();

    const std::optional<ILine*> GetLine() {
        return outgoingLine;
    }

    vector<IControl*> GetChildControls() {
        vector<IControl*> myControls;
        myControls.push_back(this->volume);
        myControls.push_back(this->pitch);
        return myControls;
    }

private: 
    void LineTo(Vector2 target);
    void LineTo(float targetX, float targetY) ;

    void DestroyAllConnections();
};