#pragma once

#include "IPlug_include_in_plug_hdr.h"

#include <optional> // we're c++17 now
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>


using std::vector;
using std::unique_ptr;
using std::unordered_map;

using namespace iplug;
using namespace igraphics;

// ---------------------------------------
// Classes

class Vector2 {
public:
    float x, y;
    Vector2() : x(0.0), y(0.0) {}
    Vector2(float x, float y) : x(x), y(y) {}
    static Vector2 Zero() {
        return Vector2();
    }
    const Vector2 operator+(const Vector2 other) const {
        return Vector2(x + other.x, y + other.y);
    }
    const Vector2& operator+=(const Vector2 other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    const Vector2 operator-() const {
        return Vector2(-x, -y);
    }
    const Vector2 operator-(const Vector2 other) const {
        return *this + -other;
    }
    const Vector2& operator-=(const Vector2 other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    const bool operator==(const Vector2 other) const {
        return x == other.x && y == other.y;
    }
    const bool operator!=(const Vector2 other) const {
        return !(*this == other);
    }
};

class Point2 {
public:
    int x, y;
    Point2() : x(0), y(0) {}
    Point2(int x, int y) { this->x = x; this->y = y; }
    static Point2 Zero() {
        return Point2();
    }
    const Point2 operator+(const Point2 other) const {
        return Point2(x + other.x, y + other.y);
    }
    const Point2& operator+=(const Point2 other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    const Point2 operator-() const {
        return Point2(-x, -y);
    }
    const Point2 operator-(const Point2 other) const {
        return *this + -other;
    }
    const Point2& operator-=(const Point2 other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    const bool operator==(const Point2 other) const {
        return x == other.x && y == other.y;
    }
    const bool operator!=(const Point2 other) const {
        return !(*this == other);
    }
};

struct point2_hash {
    std::size_t operator() (const Point2& point) const {
        return std::hash<int>()(point.x) ^ std::hash<int>()(point.y);
    }
};

struct vector2_hash {
    std::size_t operator() (const Vector2& vector) const {
        return std::hash<float>()(vector.x) ^ std::hash<float>()(vector.y);
    }
};

// ---------------------------------------
// Constants

inline float lineGap_ = 100.0, offsetX_ = 37.5, offsetY_ = 12.5, mainPadding_ = 20.0;

// ---------------------------------------
// Functions

inline Point2 LocationToIndex(Vector2 pos, float width = 0, float height = 0) {
    return Point2((int)((pos.x - width / 2 - offsetX_ - mainPadding_) / lineGap_),
                  (int)((pos.y - height / 2 - offsetY_ - mainPadding_) / lineGap_));
}

inline Vector2 IndexToLocation(Point2 loc, float width = 0, float height = 0) {
    float x = loc.x * lineGap_ + width / 2 + offsetX_ + mainPadding_;
    float y = loc.y * lineGap_ + height / 2 + offsetY_ + mainPadding_;
    return Vector2(x, y);
}

inline int GetUid() {
    static int currentId = 0;
    return currentId++;
}

inline IRECT GetRECTAt(Point2 location) {
    float x = (location.x) * lineGap_ + offsetX_ + mainPadding_;
    float y = (location.y) * lineGap_ + offsetY_ + mainPadding_;
    return IRECT(x + 8, y + 8, x + lineGap_ - 8, y + lineGap_ - 8);
}