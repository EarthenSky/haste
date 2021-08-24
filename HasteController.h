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

    void AddConnection(int parentUid, ILine* line);
    void RemoveConnection(int parentUid);
    void UpdateConnection(int parentUid, Vector2 start, Vector2 end);
    bool IsConnectionFrom(int parentUid);

};