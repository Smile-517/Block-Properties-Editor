#pragma once

#include <string>

#include "Config.h"
#include "Objects/BlockIdObj.h"

using namespace std;

class OneBlockId : public BlockIdObj {
   public:
    // Constructor
    OneBlockId();

    // Destructor
    virtual ~OneBlockId();

    // Getters
    bool isSomeWordDeleted() const;

    const string& getWord(const int index) const;
    int getWordNum() const;
    static int getAllAddedWordNum();
    static int getAllDeletedWordNum();
    static int getAllWordNum(const OneBlockId* const oneBlockIdList, const int oneBlockIdNum);

    const string& getUselessWord(const int index) const;
    int getUselessWordNum() const;
    static int getAllUselessWordNum(const OneBlockId* const oneBlockIdList, const int oneBlockIdNum);

    // Methods
    void enableUselessWordList();
    void registerLine(const int mode, string& line, OneBlockId* const resourceOneBlockIdList,
                      const int resourceOneBlockIdNum, const OneBlockId* const patchOneBlockIdList,
                      const int patchOneBlockIdNum);

   protected:
    // Member variables
    bool someWordDeleted;

    string wordList[maxWord];  // List that will store all the words for 1 block ID.
    int wordNum;               // Number of words in 1 block ID.
    static int allAddedWordNum;
    static int allDeletedWordNum;

    string* uselessWordList;
    int uselessWordNum;  // Number of words that are useless in 1 block ID(1 patcher).

    // Protected methods
    void insertWord(const int mode, const string& word, const int targetBlockId,
                    OneBlockId* const resourceOneBlockIdList, const int resourceOneBlockIdNum,
                    const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum);
    void resourceInsertWord(const string& wordIn, const int targetBlockId, OneBlockId* const resourceOneBlockIdList,
                            const int resourceOneBlockIdNum);
    void patchInsertWord(const string& wordIn, const int targetBlockId, OneBlockId* const resourceOneBlockIdList,
                         const int resourceOneBlockIdNum, const OneBlockId* const patchOneBlockIdList,
                         const int patchOneBlockIdNum);

    void deleteWord(const int index);
};