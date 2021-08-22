#pragma once

#include "common.h"
#include "IWaveBlock.h"

class Haste;

class HasteController {
private:
    Haste& haste_;

public:
    HasteController(Haste& haste) : haste_(haste) {}

    void AddWaveBlock(IWaveBlock* waveBlock, Point2 targetLoc);
    void RemoveWaveBlock(Point2 targetLoc);
    void MoveWaveBlock(Point2 oldLoc, Point2 newLoc);
    bool IsWaveBlockAt(Point2 targetLoc);
    IWaveBlock* GetWaveBlock(Point2 targetLoc);

    void AddConnection(ILine* line);
    void RemoveConnection(Vector2 start);
    void UpdateConnection(Vector2 start, Vector2 end);
    bool IsConnectionAt(Vector2 start);

};