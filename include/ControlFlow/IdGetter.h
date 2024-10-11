#pragma once

#include <string>

#include "Config.h"
#include "Objects/BlockIdObj.h"

using namespace std;

// Read resource file and make block ID list.
void registerResourceId(const string& resourceFilePath, int& resourceIdNum, BlockIdObj resourceIdList[maxBlockId]);

// Read patch files and make sorted patch block ID list.
void registerPatchId(const string* const patchFilePaths, const int patchFileNum,
                     const BlockIdObj resourceIdList[maxBlockId], BlockIdObj patchIdList[maxBlockId], int& patchIdNum);

// Print the number of ID list of patch files. If there is no ID in patch files, terminate the program.
void printPatchIdNum(const int patchIdNum, const int patchFileNum);

// Sort ID list in order of block ID
void sortIdList(BlockIdObj idList[maxBlockId], int start, int end);