#include "Tester.h"

#include <iostream>

using namespace std;

void printAllResourceIds(BlockIdObj* resourceIdList, const int resourceIdNum) {
    for (int i = 0; i < resourceIdNum; i++) {
        cout << "resourceId." << i << ": " << resourceIdList[i].getBlockId() << endl;
    }
}

void printAllPatchIds(BlockIdObj* patchIdList, const int patchIdNum) {
    for (int i = 0; i < patchIdNum; i++) {
        cout << "patchId." << i << ": " << patchIdList[i].getBlockId() << endl;
    }
}