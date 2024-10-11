#pragma once

#include "Objects/BlockIdObj.h"

// Print all the block IDs in resource file.
void printAllResourceIds(BlockIdObj* resourceIdList, const int resourceIdNum);

// Print all the block IDs in patch file(s).
void printAllPatchIds(BlockIdObj* patchIdList, const int patchIdNum);