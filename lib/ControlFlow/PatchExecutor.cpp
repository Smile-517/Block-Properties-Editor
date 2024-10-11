#include "ControlFlow/PatchExecutor.h"

#include <fstream>
#include <iostream>

using namespace std;

void doPatch(const string& resourceFilePath, const OneBlockId* const resourceOneBlockIdList,
             const int resourceOneBlockIdNum, const OneBlockId* const patchOneBlockIdList, const int patchOneBlockIdNum,
             const string& dirResult) {
    ifstream resourceFile{resourceFilePath};

    // Initialization: find altered resourceOneBlockId.
    int resourceIdCnt = 0;
    bool isResourceIdFinished = false;
    while ((!resourceOneBlockIdList[resourceIdCnt].isSomeWordDeleted()) && (!isResourceIdFinished)) {
        resourceIdCnt++;

        if (resourceIdCnt > resourceOneBlockIdNum - 1) {
            isResourceIdFinished = true;
        }
    }

    string line;
    int patchIdCnt = 0;
    int fileBlockId;
    bool isPatched;
    string res = "";
    while (getline(resourceFile, line)) {
        isPatched = false;

        if (line.substr(0, 6) == "block.") {  // line looks like "block.10012=vine"
            // Erase whitespaces in resource file's line.
            while (line[line.length() - 1] == ' ') {
                line.erase(line.length() - 1);
            }

            /* For every patcher, add words 'once' to the end of the line. Complementary uses the #if statement to
             * specify duplicate block IDs for each Minecraft version. As of version r5.1.1, block IDs for
             * Minecraft 1.13 and higher appear first, so this program only interacts with block IDs for
             * Minecraft 1.13 and higher. */
            fileBlockId = stoi(line.substr(6, 5));

            if ((!isResourceIdFinished) && (fileBlockId == resourceOneBlockIdList[resourceIdCnt].getBlockId())) {
                isPatched = true;

                line = line.substr(0, 12);  // Now, line looks like "block.10012="
                if (resourceOneBlockIdList[resourceIdCnt].getWordNum() >= 1) {
                    line += resourceOneBlockIdList[resourceIdCnt].getWord(0);

                    for (int wordCnt = 1; wordCnt < resourceOneBlockIdList[resourceIdCnt].getWordNum(); wordCnt++) {
                        line += " ";
                        line += resourceOneBlockIdList[resourceIdCnt].getWord(wordCnt);
                    }
                }

                line += "\n";
                res += line;

                resourceIdCnt++;
                if (resourceIdCnt > resourceOneBlockIdNum - 1) {
                    isResourceIdFinished = true;
                }

                if (!isResourceIdFinished) {
                    while ((!resourceOneBlockIdList[resourceIdCnt].isSomeWordDeleted()) && (!isResourceIdFinished)) {
                        resourceIdCnt++;

                        if (resourceIdCnt > resourceOneBlockIdNum - 1) {
                            isResourceIdFinished = true;
                        }
                    }
                }
            }

            if (fileBlockId == patchOneBlockIdList[patchIdCnt].getBlockId()) {
                isPatched = true;

                if (line.length() != 12) {  // line includes some words.
                    for (int wordCnt = 0; wordCnt < patchOneBlockIdList[patchIdCnt].getWordNum(); wordCnt++) {
                        line += " ";
                        line += patchOneBlockIdList[patchIdCnt].getWord(wordCnt);
                    }
                } else {  // line doesn't include any word.
                    if (patchOneBlockIdList[patchIdCnt].getWordNum() >= 1) {
                        line += patchOneBlockIdList[patchIdCnt].getWord(0);

                        for (int wordCnt = 1; wordCnt < patchOneBlockIdList[patchIdCnt].getWordNum(); wordCnt++) {
                            line += " ";
                            line += patchOneBlockIdList[patchIdCnt].getWord(wordCnt);
                        }
                    }
                }

                line += "\n";
                res += line;

                patchIdCnt++;
            }
        }
        if (!isPatched) {
            line += "\n";
            res += line;
        }
    }
    resourceFile.close();

    ofstream resultFile{dirResult + "block.properties"};
    resultFile.write(res.c_str(), res.size());

    resultFile.close();
    // Patch completed.

    // Print patch result.
    clog << "Patch process completed successfully!" << endl;

    int allAddedWordNum = OneBlockId::getAllAddedWordNum();
    int allDeletedWordNum = OneBlockId::getAllDeletedWordNum();
    int realAddedWordNum = allAddedWordNum - allDeletedWordNum;
    int movedWordNum = allDeletedWordNum;
    if (realAddedWordNum == 1) {
        clog << "Word added: " << realAddedWordNum << endl;
    } else {
        clog << "Words added: " << realAddedWordNum << endl;
    }
    if (movedWordNum == 1) {
        clog << "Word moved: " << movedWordNum << endl;
    } else {
        clog << "Words moved: " << movedWordNum << endl;
    }
}