#pragma once

#include "common.h"

#include "IControl.h"

using namespace iplug;
using namespace igraphics;

// TODO: removing connections, float connections to top layer, connections follow 
// blocks, and connections fall in center of each block, and removing a block also 
// removes ALL connections.

class ILine : public IControl {
private:
    Vector2 start_;
    Vector2 end_;

    Vector2 startTransform;
    Vector2 endTransform;

public:
    ILine(Vector2 start, Vector2 end)
    : IControl(IRECT::Zero()), 
      start_(start), end_(end) 
    {
        SetTargetRECT(IRECT::Zero());
        UpdateDrawRECT();
    }

    void Draw(IGraphics& g) override {
        start_ += startTransform;
        end_ += endTransform;
        UpdateDrawRECT();

        // TODO: implement a better way to accomplish this.
        // override iplugin's clipping rectangle so we can draw our transformed rectangle.
        auto mVG = (NVGcontext*)g.GetDrawContext();
        nvgScissor(mVG, mRECT.L, mRECT.T, mRECT.W(), mRECT.H());

        // TODO: make this go to the front?
        g.DrawLine(COLOR_WHITE, start_.x, start_.y, end_.x, end_.y, nullptr, 2.0f);

        startTransform = Vector2::Zero();
        endTransform = Vector2::Zero();
    }

    Vector2 GetStart() const {
        return start_;
    }

    void SetStart(const Vector2 start) {
        startTransform = start - start_;
        SetDirty(false);
    }
    void SetEnd(const Vector2 end) {
        endTransform = end - end_;
        SetDirty(false);
    }

    void MoveStart(float dX, float dY) {
        startTransform += Vector2(dX, dY);
        SetDirty(false);
    }
    void MoveEnd(float dX, float dY) {
        endTransform += Vector2(dX, dY);
        SetDirty(false);
    }

private:
    void UpdateDrawRECT() {
        auto l = std::min(start_.x, end_.x);
        auto t = std::min(start_.y, end_.y);
        auto r = std::max(start_.x, end_.x);
        auto b = std::max(start_.y, end_.y);
        auto newRECT = IRECT(l, t, r, b);
        SetRECT(newRECT);
    }
};