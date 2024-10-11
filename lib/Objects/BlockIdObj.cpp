#include "Objects/BlockIdObj.h"

// Constructor
BlockIdObj::BlockIdObj() : blockId(-1) {}

// Setters
void BlockIdObj::setId(int blockIdIn) { blockId = blockIdIn; }

// Getters
int BlockIdObj::getBlockId() const { return blockId; }