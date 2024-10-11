#include "ControlFlow/WordRegistrar.h"

#include <fstream>
#include <iostream>

using namespace std;

OneBlockId* makeOneBlockIdList(const int mode, const BlockIdObj idListIn[maxBlockId], const int idNumIn) {
    OneBlockId* const oneBlockIdList = new OneBlockId[idNumIn];
    if (mode == 0) {  // Mode 0 is for resource block ID.
        for (int i = 0; i < idNumIn; i++) {
            oneBlockIdList[i].setId(idListIn[i].getBlockId());
        }
    } else {  // Mode 1 is for patch block ID.
        for (int i = 0; i < idNumIn; i++) {
            oneBlockIdList[i].setId(idListIn[i].getBlockId());
            oneBlockIdList[i].enableUselessWordList();
        }
    }

    return oneBlockIdList;
}

void registerResourceWord(const string& resourceFilePath, OneBlockId* const resourceOneBlockIdList,
                          const int resourceOneBlockIdNum) {
    // Open resourceFile
    ifstream resourceFile{resourceFilePath};

    // Read all the lines in resource file and register words to resourceOneBlockIdList.
    string line;
    string targetBlockId;
    bool isRegistered;
    for (int blockIdCnt = 0; blockIdCnt < resourceOneBlockIdNum; blockIdCnt++) {
        isRegistered = false;
        while (!isRegistered) {
            getline(resourceFile, line);
            if ((line.substr(0, 6) == "block.") &&
                (stoi(line.substr(6, 5)) == resourceOneBlockIdList[blockIdCnt].getBlockId())) {
                // Remove all the useless whitespaces.
                while (line[line.length() - 1] == ' ') {
                    line.erase(line.length() - 1);
                }

                resourceOneBlockIdList[blockIdCnt].registerLine(0, line, resourceOneBlockIdList, resourceOneBlockIdNum,
                                                                nullptr, -1);
                isRegistered = true;
            }
        }
    }  // Register progress completed.

    resourceFile.close();

    clog << OneBlockId::getAllWordNum(resourceOneBlockIdList, resourceOneBlockIdNum) << " words found in resource file."
         << endl;
}

void registerPatchWord(const string* const patchFilePaths, const int patchFileNum,
                       OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
                       OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum) {
    // Read all the lines in all the patch file(s) and register words to patchOneBlockIdList.
    ifstream patchFile;
    string line;
    int targetBlockId;
    bool isWordRegistered;
    int idCnt;
    for (int i = 0; i < patchFileNum; i++) {
        patchFile.open(patchFilePaths[i], ifstream::in);

        while (getline(patchFile, line)) {
            if (line.substr(0, 6) == "block.") {
                targetBlockId = stoi(line.substr(6, 5));

                isWordRegistered = false;
                idCnt = 0;
                while (!isWordRegistered) {
                    if (targetBlockId > patchOneBlockIdList[idCnt].getBlockId()) {
                        idCnt++;
                    } else {  // targetBlockId == patchOneBlockIdList[idCnt].getBlockId(), < case is blocked by IdGetter
                        // Remove all the useless whitespaces.
                        while (line[line.length() - 1] == ' ') {
                            line.erase(line.length() - 1);
                        }

                        patchOneBlockIdList[idCnt].registerLine(1, line, resourceOneBlockIdList, resourceOneBlockIdNum,
                                                                patchOneBlockIdList, patchOneBlockIdNum);

                        isWordRegistered = true;
                    }
                }
            }
        }

        patchFile.close();
    }  // Register progress completed.

    // Print number of all the words in patch file(s) or terminate if there is no word.
    int allUsefulWordNum = OneBlockId::getAllWordNum(patchOneBlockIdList, patchOneBlockIdNum);
    int allUselessWordNum = OneBlockId::getAllUselessWordNum(patchOneBlockIdList, patchOneBlockIdNum);
    int allWordNum = allUsefulWordNum + allUselessWordNum;
    if (allWordNum == 0) {
        clog << "No word found in patch file. There is nothing to do. Terminating." << endl;
        system("pause");
        exit(0);
    } else if ((allWordNum == 1) && (patchFileNum == 1)) {
        clog << allWordNum << " word found in patch file." << endl;
    } else if ((allWordNum == 1) && (patchFileNum >= 2)) {
        clog << allWordNum << " word found in patch files." << endl;
    } else if ((allWordNum >= 2) && (patchFileNum == 1)) {
        clog << allWordNum << " words found in patch file." << endl;
    } else {
        clog << allWordNum << " words found in patch files." << endl;
    }

    // Print all the useless words due to duplication if exist.
    printAllUselessWords(patchOneBlockIdList, patchOneBlockIdNum, allUselessWordNum);
}

void printAllUselessWords(const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
                          const int allUselessWordNum) {
    if (allUselessWordNum > 0) {
        if (allUselessWordNum == 1) {
            clog << "Among them, " << allUselessWordNum
                 << " word was useless due to duplication in the same ID in resource or patch file(s). ignoring it."
                 << endl;
        } else if (allUselessWordNum >= 2) {
            clog << "Among them, " << allUselessWordNum
                 << " words were useless due to duplication in the same ID in resource or patch file(s). ignoring them."
                 << endl;
        }

        for (int idCnt = 0; idCnt < patchOneBlockIdNum; idCnt++) {
            if (patchOneBlockIdList[idCnt].getUselessWordNum() >= 1) {
                clog << "Useless word(s) in block." << patchOneBlockIdList[idCnt].getBlockId() << ":";
                for (int wordCnt = 0; wordCnt < patchOneBlockIdList[idCnt].getUselessWordNum(); wordCnt++) {
                    clog << " " << patchOneBlockIdList[idCnt].getUselessWord(wordCnt);
                }
                clog << endl;
            }
        }
    }
}