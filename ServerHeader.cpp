// Copyright Yusuf Ozdemir 2022
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include "ServerHeader.h"

using std::cout, std::endl;
using std::vector, std::ifstream, std::ofstream, std::string;

// Creates a cstring containing all of the files in library.txt in a formatted style. 
//   Returns a vector with the file names
vector<string> printFiles(int *docCount, int *totalSizeBytes, char *fileList) {
    // Opens a file reader for library.txt and initializes appropriate variables
    ifstream readLibrary("library.txt");
    string docName, auName;
    int sizebytes;
    vector<string> fileVec;

    // Reading from library.txt
    while (readLibrary >> docName) {
        // Incrementing docCount
        *docCount = *docCount + 1;

        // Reading from library.txt into variables
        readLibrary >> sizebytes;
        readLibrary >> auName;

        *totalSizeBytes += sizebytes;  // Adds current file size to totalSizeBytes

        
        if (fileVec.size() == 0 || fileVec.at(fileVec.size() - 1) != docName) {
            fileVec.push_back(docName);

            string fileInfo = std::__cxx11::to_string(*docCount) + ". " + docName + " \t"
                    + std::__cxx11::to_string(sizebytes) + " bytes. \tAuthor: " + auName + "\n";
            strcat(fileList, fileInfo.c_str());
        }
        
    }
    readLibrary.close();

    // Adds library statistics to the cstring
    string libStats = "Number of documents: " + std::__cxx11::to_string(*docCount)
            + "\nTotal Bytes in Library: " +
            std::__cxx11::to_string(*totalSizeBytes) + "\n";
    strcat(fileList, libStats.c_str());

    return fileVec;
}

// Writes a received doc to library.txt and returns the appropriate code
int receiveFile(int fileSize, char *userStringInput, char *username) {
    if (fileSize > 0) {
        // Opens library.txt and writes new file to it in proper formatting
        ofstream writeLibrary("library.txt", std::ios::app);
        writeLibrary << userStringInput << " " << fileSize << " " << username << "\n";
        writeLibrary.close();
        cout << "File '" << userStringInput << "' (" << fileSize
                << " bytes) has been added to the library by user '" << username << "'\n"; 
        // Returns 1 if successful, returns -1 otherwise
        return 1;
    } else { return -1; }
}
