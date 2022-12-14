#include "Haste.h"

// -----------------------------------
// Wave Blocks

// TODO: the caller of this function should give us a pointer to IWaveBlock, which we will then directly give to the UI, 
// instead just storing some meta data about how to access that ui & later destroy it.
void HasteController::AddWaveBlock(IRECT waveBlockRect, Point2 targetLoc) {
    const float halfBlock = (100.0 - 16.0) / 2.0;
    IRECT topRect = waveBlockRect.GetFromTop(halfBlock);
    IRECT leftRect = topRect.GetFromLeft(halfBlock);
    IRECT rightRect = topRect.GetFromRight(halfBlock);
    //IRECT pitchRect = waveBlockRect.GetFromBottom((100.0 - 16.0) / 2.0);

    auto volume = new IVSliderControl(leftRect); // TODO: figure out how to get this to work (& analyze IVNumberBoxControl to see if it does what I want & how it mananges child parent relationships.)
    auto pitch = new IVNumberBoxControl(rightRect);
    IWaveBlock* waveBlock = new IWaveBlock(*this, waveBlockRect, volume, pitch);

    IControl* control = haste_.GetUI()->AttachControl(waveBlock);
    control = haste_.GetUI()->AttachControl(volume);
    control = haste_.GetUI()->AttachControl(pitch);

    haste_.blockLocationMap[targetLoc] = waveBlock;
}

void HasteController::RemoveWaveBlock(Point2 targetLoc) {
    auto waveBlock = haste_.blockLocationMap[targetLoc];
    haste_.blockLocationMap.erase(targetLoc);
    haste_.GetUI()->RemoveControl(waveBlock);

    // Also remove child controls.
    // TODO: figure out how to properly do parent child relationships.
    for (IControl* control : waveBlock->GetChildControls()) {
        haste_.GetUI()->RemoveControl(control);
    }
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