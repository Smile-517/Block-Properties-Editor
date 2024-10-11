#pragma once

#include <string>

using namespace std;

// Count the number of files in given directory.
int countFiles(const string& path);

// Get resource file path. Only one file is allowed. Return path of resource file.
const string getResourceFilePath(const string& path);

// Get patch file path. Makes fileNum correct and return paths of patch files.
const string* const getPatchFilePaths(const string& path, int& fileNum);

// Check if there is block.properties file in the result folder. If so, let the user decide whether to overwrite it.
double checkResultFile(const string& path);