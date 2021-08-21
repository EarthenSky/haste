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

    void AddWaveBlock(IWaveBlock* waveBlock, int_pair targetLoc);
    void RemoveWaveBlock(int_pair targetLoc);
    void MoveWaveBlock(int_pair oldLoc, int_pair newLoc);
    bool IsWaveBlockAt(int_pair targetLoc);

    void CreateConnection(int_pair start, int_pair end) {

    }

    void DestroyConnection(int_pair start, int_pair end) {

    }
};