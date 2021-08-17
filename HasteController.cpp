#include "Haste.h"

// TODO: the caller of this function should give us a pointer to IWaveBlock, which we will then directly give to the UI, 
// instead just storing some meta data about how to access that ui & later destroy it.
void HasteController::AddWaveBlock(unique_ptr<IWaveBlock>&& waveBlock, int_pair targetLoc) {
    haste_.GetUI()->AttachControl(waveBlock.get());
        
    haste_.blockLocationMap[targetLoc] = true;
    haste_.waveBlocks.push_back(std::move(waveBlock));
}

void HasteController::RemoveWaveBlock(int_pair targetLoc) {
    haste_.blockLocationMap[targetLoc] = false;
    // TODO: this needs a proper remove (remove vector first)
}

bool HasteController::IsWaveBlockAt(int_pair targetLoc) {
    return haste_.blockLocationMap.find(targetLoc) == haste_.blockLocationMap.end();
}