#pragma once

#include "common.h"
#include "ILine.h"

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class HasteController;

// TODO: set grid as a "peer"
class IWaveBlock : public IControl {
public:
    HasteController& controller_;

private:
    float transformedX = 0.0, transformedY = 0.0;
    Point2 myLocation;

    bool dragging = false;

    // we're small enough that this vector will never get to be more than a few hundred connections
    vector<IWaveBlock*> ingoingConnections;
    std::optional<IWaveBlock*> outgoingConnection;
    std::optional<ILine*> outgoingLine;
    
public:
    IWaveBlock(HasteController& controller, const IRECT& bounds)
    : controller_(controller), IControl(bounds) 
    {
        myLocation = CurrentLocation();
        outgoingLine = std::nullopt;
    }

    Point2 CurrentLocation() {
        const IRECT& bounds = GetRECT();
        return Point2((int)((bounds.L + bounds.W()/2 - offsetX_ - mainPadding_) / lineGap_),
                      (int)((bounds.T + bounds.H()/2 - offsetY_ - mainPadding_) / lineGap_));
    }
    
    // TODO: make this a utility function
    IRECT GetRECTAt(Point2 location) {
        float x = (location.x) * lineGap_ + offsetX_ + mainPadding_;
        float y = (location.y) * lineGap_ + offsetY_ + mainPadding_;
        return IRECT(x + 8, y + 8, x + lineGap_ - 8, y + lineGap_ - 8);
    }

    void Draw(IGraphics& g) override;
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    void OnMouseUp(float x, float y, const IMouseMod& mod) override;

    void AddIngoingConnection(IWaveBlock* waveBlock) {
        ingoingConnections.push_back(waveBlock);
    }

    void RemoveIngoingConnection(IWaveBlock* toRemove) {
        // TODO: find toRemove, & remove it
        // TODO: how should the UI look for removing connections?
    }

private: 
    void LineTo(float targetX, float targetY) ;

    void EndLine();
    
    void CreateConnection(IWaveBlock* target, Vector2 start, Vector2 end);
    void DestroyConnection();
};