#pragma once

#include "utility.h"

// TODO: make a common.h

#include "IWaveBlock.h"

using std::vector;
using std::unique_ptr;

using namespace iplug;
using namespace igraphics;

class Haste;

class HasteController {
private:
    Haste& haste_;

public:
    HasteController(Haste& haste)
        : haste_(haste) {}

    void AddWaveBlock(unique_ptr<IWaveBlock>&& waveBlock, int_pair targetLoc);
    void RemoveWaveBlock(int_pair targetLoc);
    bool IsWaveBlockAt(int_pair targetLoc);
    
    void MoveWaveBlockTo(int_pair originalLoc, float x, float y) {

    }

    void CreateConnection(int_pair start, int_pair end) {

    }

    void DestroyConnection(int_pair start, int_pair end) {

    }
};