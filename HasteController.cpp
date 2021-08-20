#include "Haste.h"

// TODO: the caller of this function should give us a pointer to IWaveBlock, which we will then directly give to the UI, 
// instead just storing some meta data about how to access that ui & later destroy it.
void HasteController::AddWaveBlock(IWaveBlock* waveBlock, int_pair targetLoc) {
    IControl* control = haste_.GetUI()->AttachControl(waveBlock);
    haste_.blockLocationMap[targetLoc] = waveBlock; // = control;
    //haste_.waveBlocks.push_back(std::move(waveBlock));
}

void HasteController::RemoveWaveBlock(int_pair targetLoc) {
    auto waveBlock = haste_.blockLocationMap[targetLoc];
    haste_.blockLocationMap[targetLoc] = nullptr; // TODO: how to remove this?
    haste_.GetUI()->RemoveControl(waveBlock);
}

bool HasteController::IsWaveBlockAt(int_pair targetLoc) {
    return haste_.blockLocationMap.find(targetLoc) == haste_.blockLocationMap.end();
}