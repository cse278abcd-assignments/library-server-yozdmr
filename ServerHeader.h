// Copyright Yusuf Ozdemir 2022
#include <vector>
#include <string>
#include <cstring>

#ifndef ASSIGNMENT3_YOZDMR_SERVERHEADER_H_
#define ASSIGNMENT3_YOZDMR_SERVERHEADER_H_

// Creates a cstring containing all of the files in library.txt in a formatted style. 
//   Returns a vector with the file names
std::vector<std::string> printFiles(int *docCount, int *totalSizeBytes, char *fileList);

// Writes a received doc to library.txt and returns the appropriate code
int receiveFile(int fileSize, char *userStringInput, char *username);

// Removes a document based on its index in the fileVec vector
void removeDoc(int userInput, std::vector<std::string> fileVec);

#endif // ASSIGNMENT3_YOZDMR_SERVERHEADER_H_