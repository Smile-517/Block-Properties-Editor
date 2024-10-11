#include "Objects/OneBlockId.h"

#include <iostream>

using namespace std;

// Initialization of static members
int OneBlockId::allAddedWordNum = 0;
int OneBlockId::allDeletedWordNum = 0;

// Constructor
OneBlockId::OneBlockId()
    : BlockIdObj(), someWordDeleted(false), wordNum(0), uselessWordList(nullptr), uselessWordNum(0) {}

// Destructor
OneBlockId::~OneBlockId() {
    if (uselessWordList != nullptr) {
        delete[] uselessWordList;
        uselessWordList = nullptr;
    }
}

// Getters
bool OneBlockId::isSomeWordDeleted() const { return someWordDeleted; }

const string& OneBlockId::getWord(const int index) const { return wordList[index]; }
int OneBlockId::getWordNum() const { return wordNum; }
int OneBlockId::getAllAddedWordNum() { return allAddedWordNum; }
int OneBlockId::getAllDeletedWordNum() { return allDeletedWordNum; }
int OneBlockId::getAllWordNum(const OneBlockId* const oneBlockIdList, const int oneBlockIdNum) {
    int allWordNum = 0;
    for (int i = 0; i < oneBlockIdNum; i++) {
        allWordNum += oneBlockIdList[i].getWordNum();
    }
    return allWordNum;
}

const string& OneBlockId::getUselessWord(const int index) const { return uselessWordList[index]; }
int OneBlockId::getUselessWordNum() const { return uselessWordNum; }
int OneBlockId::getAllUselessWordNum(const OneBlockId* const oneBlockIdList, const int oneBlockIdNum) {
    int allUselessWordNum = 0;
    for (int idCnt = 0; idCnt < oneBlockIdNum; idCnt++) {
        allUselessWordNum += oneBlockIdList[idCnt].getUselessWordNum();
    }
    return allUselessWordNum;
}

// Methods
void OneBlockId::enableUselessWordList() { uselessWordList = new string[maxWord]; }

void OneBlockId::registerLine(const int mode, string& line, OneBlockId* const resourceOneBlockIdList,
                              const int resourceOneBlockIdNum, const OneBlockId* const patchOneBlockIdList,
                              const int patchOneBlockIdNum) {  // line looks like "block.10012=vine dirt stone"
    int targetBlockId = stoi(line.substr(6, 5));
    line.erase(0, 12);  // And now, line looks like "vine dirt stone"

    int spacePos = line.find(" ");
    int cutLineLength = line.length();
    if (spacePos != string::npos) {  // There are some words (2 or more).
        int cursorPos = 0;
        int wordLength;
        string word;
        while (spacePos != string::npos) {  // Search for all whitespaces.
            wordLength = spacePos - cursorPos;

            word = line.substr(cursorPos, wordLength);
            insertWord(mode, word, targetBlockId, resourceOneBlockIdList, resourceOneBlockIdNum, patchOneBlockIdList,
                       patchOneBlockIdNum);

            cursorPos = spacePos + 1;
            spacePos = line.find(" ", cursorPos);
        }

        // Register last word.
        if (cursorPos < cutLineLength) {  // So, "vine dirt stone " is not the case.
            word = line.substr(cursorPos);
            insertWord(mode, word, targetBlockId, resourceOneBlockIdList, resourceOneBlockIdNum, patchOneBlockIdList,
                       patchOneBlockIdNum);
        }
    } else if ((spacePos == string::npos) && (cutLineLength != 0)) {  // There is 1 word.
        insertWord(mode, line, targetBlockId, resourceOneBlockIdList, resourceOneBlockIdNum, patchOneBlockIdList,
                   patchOneBlockIdNum);
    }  // If there is nothing, nothing happens.
}

// Protected methods
void OneBlockId::insertWord(const int mode, const string& word, const int targetBlockId,
                            OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum,
                            const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum) {
    if (mode == 0) {  // Mode 0 is resource mode.
        resourceInsertWord(word, targetBlockId, resourceOneBlockIdList, resourceOneBlockIdNum);
    } else {  // Mode 1 is patch mode.
        patchInsertWord(word, targetBlockId, resourceOneBlockIdList, resourceOneBlockIdNum, patchOneBlockIdList,
                        patchOneBlockIdNum);
    }
}

void OneBlockId::resourceInsertWord(const string& wordIn, const int targetBlockId,
                                    OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum) {
    // Check for duplicated words in resource file.
    for (int idCnt = 0; idCnt < resourceOneBlockIdNum; idCnt++) {
        for (int wordCnt = 0; wordCnt < resourceOneBlockIdList[idCnt].getWordNum(); wordCnt++) {
            if (wordIn == resourceOneBlockIdList[idCnt].getWord(wordCnt)) {
                if (resourceOneBlockIdList[idCnt].getBlockId() != targetBlockId) {
                    cerr << "ERROR: A word \'" << wordIn << "\' is in both block."
                         << resourceOneBlockIdList[idCnt].getBlockId() << " and block." << targetBlockId
                         << " in resource file. Please resolve the duplication in the resource file!" << endl;
                    system("pause");
                    exit(1);
                } else {  // resourceOneBlockIdList[idCnt].getBlockId() == targetBlockId
                    cerr << "ERROR: A word \'" << wordIn << "\' in block." << targetBlockId
                         << " in resource file is duplicated. Please resolve the duplication in the resource file!"
                         << endl;
                    system("pause");
                    exit(1);
                }
            }
        }
    }

    wordList[wordNum] = wordIn;
    wordNum++;

    if (wordNum > maxWord - 1) {
        cerr << "ERROR: Too many words at block." << blockId << " in resource file! (max: " << maxWord << ")" << endl;
        system("pause");
        exit(1);
    }
}

void OneBlockId::patchInsertWord(const string& wordIn, const int targetBlockId,
                                 OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum,
                                 const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum) {
    bool needSendUseless = false;
    // Check for duplicated words in resource file.
    for (int idCnt = 0; idCnt < resourceOneBlockIdNum; idCnt++) {
        for (int wordCnt = 0; wordCnt < resourceOneBlockIdList[idCnt].getWordNum(); wordCnt++) {
            if (wordIn == resourceOneBlockIdList[idCnt].getWord(wordCnt)) {
                // If words in resource file and patch file are in same ID, it will send this to useless word.
                // If words in resource file and patch file are in different ID, it will delete that word in
                // resource file, and insert it in new block ID.
                if (targetBlockId == resourceOneBlockIdList[idCnt].getBlockId()) {
                    needSendUseless = true;
                } else {
                    resourceOneBlockIdList[idCnt].deleteWord(wordCnt);
                }
            }
        }
    }

    // Check for duplicated words in patch file(s).
    for (int idCnt = 0; idCnt < patchOneBlockIdNum; idCnt++) {
        for (int wordCnt = 0; wordCnt < patchOneBlockIdList[idCnt].getWordNum(); wordCnt++) {
            if (wordIn == patchOneBlockIdList[idCnt].getWord(wordCnt)) {
                // If words in each patch files are in same ID, it will send this to useless word.
                // If words in each patch files are in different ID, it will throw error.
                if (targetBlockId == patchOneBlockIdList[idCnt].getBlockId()) {
                    needSendUseless = true;
                } else {
                    cerr << "ERROR: A word \'" << wordIn << "\' is in both block."
                         << patchOneBlockIdList[idCnt].getBlockId() << " and block." << targetBlockId
                         << " in patch file(s). This program can't decide which of these words should patched!" << endl;
                    system("pause");
                    exit(1);
                }
            }
        }
    }

    // Check for duplicated words in uselessWordList for avoiding duplicating words in different IDs in patch files.
    // If found, it will throw error.
    for (int idCnt = 0; idCnt < patchOneBlockIdNum; idCnt++) {
        for (int wordCnt = 0; wordCnt < patchOneBlockIdList[idCnt].getUselessWordNum(); wordCnt++) {
            // Since a word is useless is equivalent to being duplicated on the same ID, only duplicates on other
            // IDs are checked.
            if ((wordIn == patchOneBlockIdList[idCnt].getUselessWord(wordCnt)) &&
                (targetBlockId != patchOneBlockIdList[idCnt].getBlockId())) {
                cerr << "ERROR: A word \'" << wordIn << "\' is in both block."
                     << patchOneBlockIdList[idCnt].getBlockId() << " and block." << targetBlockId
                     << " in patch file(s). This program can't decide which of these words should patched!" << endl;
                system("pause");
                exit(1);
            }
        }
    }

    // If the word need to be sent to useless word list, send it. Else, register word.
    if (needSendUseless) {
        uselessWordList[uselessWordNum] = wordIn;
        uselessWordNum++;

        if (uselessWordNum > maxWord - 1) {
            cerr << "ERROR: Too many useless words due to duplication in same ID at block." << blockId
                 << " in some patch file(s)! (max: " << maxWord << ")" << endl;
            system("pause");
            exit(1);
        }
    } else {
        wordList[wordNum] = wordIn;
        wordNum++;
        allAddedWordNum++;

        if (wordNum > maxWord - 1) {
            cerr << "ERROR: Too many words at block." << blockId << " in some patch file(s)! (max: " << maxWord << ")"
                 << endl;
            system("pause");
            exit(1);
        }
    }
}

void OneBlockId::deleteWord(const int index) {
    if (wordNum >= 1) {
        someWordDeleted = true;
        for (int i = index + 1; i < wordNum; i++) {
            wordList[i - 1] = wordList[i];
        }
        wordNum--;
        allDeletedWordNum++;
    }
}