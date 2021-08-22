#pragma once

#include "common.h"

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

class ILine : public IControl {
private:
    Vector2 start_;
    Vector2 end_;

public:
    ILine(Vector2 start, Vector2 end)
    : IControl(IRECT( std::min(start.x, end.x), std::min(start.y, end.y), std::max(start.x, end.x), std::max(start.y, end.y) )), 
      start_(start), end_(end) 
    {
        SetTargetRECT(IRECT::Zero());
    }

    void Draw(IGraphics& g) override {
        // TODO: make this go to the front?
        g.DrawLine(COLOR_WHITE, start_.x, start_.y, end_.x, end_.y, nullptr, 2.0f);
    }

    Vector2 GetStart() const {
        return start_;
    }

    void SetEnd(const Vector2 end) {
        end_ = end;
    }
};