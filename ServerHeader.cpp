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

        // Adds file name to vector
        fileVec.push_back(docName);

        // Adds properly formatted string of file information to cstring
        string fileInfo = std::__cxx11::to_string(*docCount) + ". " + docName + " \t"
                + std::__cxx11::to_string(sizebytes) + " bytes. \tAuthor: " + auName + "\n";
        strcat(fileList, fileInfo.c_str());
        
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

// Removes a document based on its index in the fileVec vector
void removeDoc(int userInput, vector<string> fileVec) {
    // Copy every line from library.txt into a vector
    vector<string> tempStoreFile;
    ifstream readLibrary("library.txt");
    string line;
    while (getline(readLibrary, line)) {
        tempStoreFile.push_back(line);
    }
    readLibrary.close();

    // Write vector data into new file EXCEPT for pos that userData - 1 is
    //   at, which is the line that the file to be deleted is in
    ofstream temp("temp_lib.txt");
    for (int i = 0; i < tempStoreFile.size(); i++) {
        if (i != userInput - 1) {
            temp << tempStoreFile.at(i) << endl;
        }
    }
    temp.close();

    // Remove library.txt and rename temp_lib.txt to library.txt
    remove("library.txt");
    rename("temp_lib.txt", "library.txt");

    // Serverside message
    cout << "Document '" << fileVec.at(userInput - 1)
            << "' has been deleted." << endl;
}