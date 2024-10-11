#include "ControlFlow/PathGetter.h"

#include <ctime>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int countFiles(const string& path) {
    int res = 0;
    fs::path p = path;
    fs::directory_iterator iter(p);

    while (iter != fs::end(iter)) {
        /* In Windows, when you change the properties of a folder, the information is saved in the Desktop.ini file.
        We need to ignore it.*/
        if (iter->path().generic_string().substr(iter->path().generic_string().length() - 4, 4) != ".ini") {
            res += 1;
        }
        iter++;
    }

    return res;
}

const string getResourceFilePath(const string& path) {
    int fileNum = countFiles(path);

    if (fileNum == 0) {
        cerr << "ERROR: No resource file is detected. 1 resource file is required!" << endl;
        system("pause");
        exit(1);
    } else if (fileNum == 1) {
        clog << "1 resource file is detected." << endl;

        fs::path p = path;
        fs::directory_iterator iter(p);

        bool pathRegistered = false;
        string res;
        while (!pathRegistered) {
            if (iter->path().generic_string().substr(iter->path().generic_string().length() - 4, 4) != ".ini") {
                res = iter->path().generic_string();
                pathRegistered = true;
            } else {
                iter++;
            }
        }

        return res;
    } else {
        cerr << "ERROR: Only 1 resource file is allowed. There are " << fileNum << " resource files existing!" << endl;
        system("pause");
        exit(1);
    }
}

const string* const getPatchFilePaths(const string& path, int& fileNum) {
    fileNum = countFiles(path);

    if (fileNum == 0) {  // No file detected. Terminating.
        clog << "No patch file is detected. There is nothing to do. Terminating." << endl;
        system("pause");
        exit(0);
    } else {  // Some file(s) detected.
        // Output log.
        if (fileNum == 1) {
            clog << fileNum << " patch file is detected." << endl;
        } else {
            clog << fileNum << " patch files are detected." << endl;
        }

        fs::path p = path;
        fs::directory_iterator iter(p);
        string* const res = new string[fileNum];
        int count = 0;

        while (iter != fs::end(iter)) {  // Read all directories in the folder.
            if (iter->path().generic_string().substr(iter->path().generic_string().length() - 4, 4) != ".ini") {
                res[count] = iter->path().generic_string();
                count += 1;
            }
            iter++;
        }

        return res;
    }
}

double checkResultFile(const string& path) {
    fs::path p = path;
    fs::directory_iterator iter(p);

    string pathOfResult = path + "block.properties";

    clock_t startTime, endTime;
    bool isBlockPropertiesDetected = false;
    while (iter != fs::end(iter)) {                           // Read all directories in the folder.
        if (iter->path().generic_string() == pathOfResult) {  // if path/block.properties detected
            isBlockPropertiesDetected = true;
            clog << "block.properties file in result directory is detected. Input y or Y to overwrite the result into "
                    "this file. Otherwise, type anything else to exit the program."
                 << endl
                 << "Input: ";

            string userInput;
            startTime = clock();
            cin >> userInput;
            endTime = clock();

            if (!(userInput == "y" || userInput == "Y")) {  // if user doesn't want to proceed
                clog << "Program terminated." << endl;
                system("pause");
                exit(0);
            }  // If user inputted y or Y, this function doesn't have any effect.
        }
        iter++;
    }

    if (isBlockPropertiesDetected) {
        return endTime - startTime;
    } else {
        return 0;
    }
}