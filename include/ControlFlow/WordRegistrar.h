#pragma once

#include "Config.h"
#include "Objects/BlockIdObj.h"
#include "Objects/OneBlockId.h"

// Make OneBlockId list based on input block ID list.
OneBlockId* makeOneBlockIdList(const int mode, const BlockIdObj idListIn[maxBlockId], const int idNumIn);

// Read resource file and register words to block IDs.
void registerResourceWord(const string& resourceFilePath, OneBlockId* const resourceOneBlockIdList,
                          const int resourceOneBlockIdNum);

// Read patch file(s) and register word(s) to block IDs.
void registerPatchWord(const string* const patchFilePaths, const int patchFileNum,
                       OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
                       OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum);

// Print all the useless words due to duplication.
void printAllUselessWords(const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
                          const int allUselessWordNum);