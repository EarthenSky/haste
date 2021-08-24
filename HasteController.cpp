#include "Haste.h"

// -----------------------------------
// Wave Blocks

// TODO: the caller of this function should give us a pointer to IWaveBlock, which we will then directly give to the UI, 
// instead just storing some meta data about how to access that ui & later destroy it.
void HasteController::AddWaveBlock(IWaveBlock* waveBlock, Point2 targetLoc) {
    IControl* control = haste_.GetUI()->AttachControl(waveBlock);
    haste_.blockLocationMap[targetLoc] = waveBlock;
}

void HasteController::RemoveWaveBlock(Point2 targetLoc) {
    auto waveBlock = haste_.blockLocationMap[targetLoc];
    haste_.blockLocationMap.erase(targetLoc);
    haste_.GetUI()->RemoveControl(waveBlock);
}

void HasteController::MoveWaveBlock(Point2 oldLoc, Point2 newLoc) {
    auto waveBlock = haste_.blockLocationMap[oldLoc];
    haste_.blockLocationMap.erase(oldLoc);
    haste_.blockLocationMap[newLoc] = waveBlock;
}

bool HasteController::IsWaveBlockAt(Point2 targetLoc) {
    return haste_.blockLocationMap.find(targetLoc) != haste_.blockLocationMap.end();
}

// use IsWaveBlockAt before using this function. We don't do any checks!
IWaveBlock* HasteController::GetWaveBlock(Point2 targetLoc) {
    return haste_.blockLocationMap[targetLoc];
}

// -----------------------------------
// Connections

void HasteController::AddConnection(int parentUid, ILine* line) {
    IControl* control = haste_.GetUI()->AttachControl(line);
    haste_.connectionMap[parentUid] = line;
}

void HasteController::RemoveConnection(int parentUid) {
    auto line = haste_.connectionMap[parentUid];
    haste_.connectionMap.erase(parentUid);
    haste_.GetUI()->RemoveControl(line);
}

void HasteController::UpdateConnection(int parentUid, Vector2 start, Vector2 end) {
    haste_.connectionMap[parentUid]->SetEnd(end);
}

bool HasteController::IsConnectionFrom(int parentUid) {
    return haste_.connectionMap.find(parentUid) != haste_.connectionMap.end();
}