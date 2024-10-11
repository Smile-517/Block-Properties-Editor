#include "ControlFlow/IdGetter.h"

#include <fstream>
#include <iostream>

using namespace std;

void registerResourceId(const string& resourceFilePath, int& resourceIdNum, BlockIdObj resourceIdList[maxBlockId]) {
    ifstream resourceFile{resourceFilePath};  // One resource file is opened.

    int blockId = 0;
    string line;
    while (getline(resourceFile, line)) {  // Read all the lines in resourceFile, line is read.

        // If the start of the line is "block.", and no duplicate confirmed, register block ID.
        if ((line.substr(0, 6) == "block.") && (blockId < stoi(line.substr(6, 5)))) {
            if (!(isdigit(line[6])) || !(isdigit(line[7])) || !(isdigit(line[8])) || !(isdigit(line[9])) ||
                !(isdigit(line[10])) || (line[11] != '=') || (line[12] == ' ')) {  // Syntax error catcher
                cerr << "ERROR: Some content(s) in resource file have invalid format!" << endl;
                system("pause");
                exit(1);
            }

            blockId = stoi(line.substr(6, 5));
            resourceIdList[resourceIdNum].setId(blockId);
            resourceIdNum++;

            if (resourceIdNum > maxBlockId - 1) {  // Error catcher.
                cerr << "ERROR: Too many block IDs in resource file! (max: " << maxBlockId << ")" << endl;
                system("pause");
                exit(1);
            }
        }
    }

    // Print number of block IDs in resource file.
    clog << "There are " << resourceIdNum << " block IDs in resource file." << endl;

    resourceFile.close();
}

void registerPatchId(const string* const patchFilePaths, const int patchFileNum,
                     const BlockIdObj resourceIdList[maxBlockId], BlockIdObj patchIdList[maxBlockId], int& patchIdNum) {
    ifstream patchFile;
    string line;
    int targetBlockId;
    for (int i = 0; i < patchFileNum; i++) {              // For each patch files,
        patchFile.open(patchFilePaths[i], ifstream::in);  // Open it

        while (getline(patchFile, line)) {        // Read all the lines in one patch file, line is read.
            if (line.substr(0, 6) == "block.") {  // if start of the line is "block."
                if (!(isdigit(line[6])) || !(isdigit(line[7])) || !(isdigit(line[8])) || !(isdigit(line[9])) ||
                    !(isdigit(line[10])) || (line[11] != '=') || (line[12] == ' ')) {  // Syntax error catcher
                    cerr << "ERROR: Some content(s) in patch file have invalid format!" << endl;
                    system("pause");
                    exit(1);
                }

                targetBlockId = stoi(line.substr(6, 5));

                bool isIdFoundInResource = false;
                int resourceIdCnt = 0;
                bool foundUselessId;
                int patchIdCnt;
                while (!isIdFoundInResource) {
                    if (targetBlockId > resourceIdList[resourceIdCnt].getBlockId()) {
                        resourceIdCnt++;
                    } else if (targetBlockId == resourceIdList[resourceIdCnt].getBlockId()) {
                        isIdFoundInResource = true;

                        // Find for same ID registered.
                        foundUselessId = false;
                        patchIdCnt = 0;
                        while (!foundUselessId && patchIdCnt < patchIdNum) {
                            if (targetBlockId == patchIdList[patchIdCnt].getBlockId()) {
                                foundUselessId = true;
                            }
                            patchIdCnt++;
                        }

                        // If patch ID is not duplicated, register it.
                        if (!foundUselessId) {
                            patchIdList[patchIdNum].setId(targetBlockId);  // So, patchIdList is NOT sorted.

                            patchIdNum++;

                            if (patchIdNum > maxBlockId - 1) {  // Error catcher.
                                if (patchFileNum == 1) {
                                    cerr << "ERROR: Too many block IDs in patch file! (max: " << maxBlockId << ")"
                                         << endl;
                                } else {
                                    cerr << "ERROR: Too many block IDs in patch files! (max: " << maxBlockId << ")"
                                         << endl;
                                }
                                system("pause");
                                exit(1);
                            }
                        }
                    } else {  // targetBlockId < resourceIdList[resourceIdCnt].getBlockId()
                        cerr << "ERROR: Patch file \'" << patchFilePaths[i] << "\' has block." << targetBlockId
                             << " that resource file doesn't have!" << endl;
                        system("pause");
                        exit(1);
                    }
                }
            }
        }

        patchFile.close();
    }  // Register progress completed.

    // Print the number of ID list of patch files. If there is no ID in patch files, terminate the program.
    printPatchIdNum(patchIdNum, patchFileNum);

    // Sort patchIdNum in order of block ID
    sortIdList(patchIdList, 0, patchIdNum - 1);
}

void printPatchIdNum(const int patchIdNum, const int patchFileNum) {
    if (patchIdNum == 1 && patchFileNum == 1) {
        clog << "There is " << patchIdNum << " block ID in patch file." << endl;
    } else if (patchIdNum == 1 && patchFileNum >= 2) {
        clog << "There is " << patchIdNum << " block ID in patch files." << endl;
    } else if (patchIdNum >= 2 && patchFileNum == 1) {
        clog << "There are " << patchIdNum << " block IDs in patch file." << endl;
    } else if (patchIdNum >= 2 && patchFileNum >= 2) {
        clog << "There are " << patchIdNum << " block IDs in patch files." << endl;
    } else if (patchIdNum <= 0 && patchFileNum == 1) {
        clog << "There is no block ID in patch file. There is noting to do. Terminating." << endl;
        system("pause");
        exit(0);
    } else {
        clog << "There is no block ID in patch files. There is noting to do. Terminating." << endl;
        system("pause");
        exit(0);
    }
}

void sortIdList(BlockIdObj idList[maxBlockId], int start, int end) {  // Quick sort
    if (start >= end) {                                               // 원소가 1개인 경우
        return;
    }

    int pivot = start;
    int i = pivot + 1;  // 왼쪽 출발 지점
    int j = end;        // 오른쪽 출발 지점
    BlockIdObj temp;

    while (i <= j) {
        // 포인터가 엇갈릴때까지 반복
        while (i <= end && idList[i].getBlockId() <= idList[pivot].getBlockId()) {
            i++;
        }
        while (j > start && idList[j].getBlockId() >= idList[pivot].getBlockId()) {
            j--;
        }

        if (i > j) {
            // 엇갈림
            temp = idList[j];
            idList[j] = idList[pivot];
            idList[pivot] = temp;
        } else {
            // i번째와 j번째를 스왑
            temp = idList[i];
            idList[i] = idList[j];
            idList[j] = temp;
        }
    }

    // 분할 계산
    sortIdList(idList, start, j - 1);
    sortIdList(idList, j + 1, end);
}