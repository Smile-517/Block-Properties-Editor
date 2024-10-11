#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>

#include "Config.h"
#include "ControlFlow/IdGetter.h"
#include "ControlFlow/PatchExecutor.h"
#include "ControlFlow/PathGetter.h"
#include "ControlFlow/WordRegistrar.h"
#include "Objects/BlockIdObj.h"
#include "Objects/OneBlockId.h"
#include "Tester.h"

using namespace std;

int main() {
    clock_t startTime = clock();
    clock_t endTime;
    double duration;

    // ================================ Start of PathGetter ================================
    // Open resource folder.
    const string resourceFilePath = getResourceFilePath(dirResource);

    // Open patch folder.
    int patchFileNum = 0;
    const string* const patchFilePaths = getPatchFilePaths(dirPatch, patchFileNum);

    // Check if the block.properties file exists in result folder.
    double timeConsumed = checkResultFile(dirResult);
    // ================================= End of PathGetter =================================

    // ================================ Start of IdGetter ================================
    clog << '\n';

    // Read resource file and make resource block ID list.
    int resourceIdNum = 0;
    BlockIdObj resourceIdList[maxBlockId];  // So, this program supports up to (maxBlockId) block IDs.
    registerResourceId(resourceFilePath, resourceIdNum, resourceIdList);

    // TEST: Print all the block IDs in resource file.
    // printAllResourceIds(resourceIdList, resourceIdNum);

    // Read patch files and make sorted patch block ID list.
    int patchIdNum = 0;
    BlockIdObj patchIdList[maxBlockId];
    registerPatchId(patchFilePaths, patchFileNum, resourceIdList, patchIdList, patchIdNum);

    // TEST: Print all the block IDs in patch file(s).
    // printAllPatchIds(patchIdList, patchIdNum);
    // ================================= End of IdGetter =================================

    // ================================ Start of WordRegistrar ================================
    clog << '\n';

    // Make resource block ID list based on resource BlockIdObj list.
    int resourceOneBlockIdNum = resourceIdNum;
    OneBlockId* resourceOneBlockIdList = makeOneBlockIdList(0, resourceIdList, resourceIdNum);

    // Make patch block ID list based on patch BlockIdObj list.
    int patchOneBlockIdNum = patchIdNum;
    OneBlockId* patchOneBlockIdList = makeOneBlockIdList(1, patchIdList, patchIdNum);

    // Read resource file and register words to block IDs.
    registerResourceWord(resourceFilePath, resourceOneBlockIdList, resourceOneBlockIdNum);

    // Read patch file(s) and register word(s) to block IDs.
    registerPatchWord(patchFilePaths, patchFileNum, patchOneBlockIdList, patchOneBlockIdNum, resourceOneBlockIdList,
                      resourceOneBlockIdNum);
    // ================================= End of WordRegistrar =================================

    // ================================ Start of PatchExecutor ================================
    clog << '\n';

    // Do auto patch and write it in result folder.
    doPatch(resourceFilePath, resourceOneBlockIdList, resourceOneBlockIdNum, patchOneBlockIdList, patchOneBlockIdNum,
            dirResult);
    // ================================= End of PatchExecutor =================================

    delete[] patchFilePaths;
    delete[] resourceOneBlockIdList;
    delete[] patchOneBlockIdList;

    endTime = clock();
    duration = endTime - startTime - timeConsumed;
    clog << "Time elapsed: " << duration << "ms" << endl;

    system("pause");
    return 0;
}