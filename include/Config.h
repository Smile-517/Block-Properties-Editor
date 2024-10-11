#pragma once

#include <string>

using namespace std;

// Set the maximum number of block IDs.
static const int maxBlockId = 1024;

// Set the maximum number of patches(words, blocks) and allowed useless words due to duplication for each block ID.
static const int maxWord = 1024;

// Set resource directory.
static const string dirResource = "./1-1_Resource/";

// Set patch directory.
static const string dirPatch = "./1-2-1_Enabled_patches/";

// Set result directory.
static const string dirResult = "./1-3_Result/";