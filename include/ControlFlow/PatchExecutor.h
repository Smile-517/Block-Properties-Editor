#pragma once

#include "Objects/OneBlockId.h"

using namespace std;

// Do auto patch and write it in result folder.
void doPatch(const string& resourceFilePath, const OneBlockId* const resourceOneBlockIdList,
             const int resourceOneBlockIdNum, const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
             const string& dirResult);