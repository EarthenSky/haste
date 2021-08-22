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

void HasteController::AddConnection(ILine* line) {
    IControl* control = haste_.GetUI()->AttachControl(line);
    haste_.connectionMap[line->GetStart()] = line; // TODO: change to Point2D?
}

void HasteController::RemoveConnection(Vector2 start) {
    auto line = haste_.connectionMap[start];
    haste_.connectionMap.erase(start);
    haste_.GetUI()->RemoveControl(line);
}

void HasteController::UpdateConnection(Vector2 start, Vector2 end) {
    haste_.connectionMap[start]->SetEnd(end);
    haste_.connectionMap[start]->SetDirty(false);
}

bool HasteController::IsConnectionAt(Vector2 start) {
    return haste_.connectionMap.find(start) != haste_.connectionMap.end();
}