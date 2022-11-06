/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
*/
// Author Yusuf Ozdemir 2022

#include <iostream>           // For cerr and cout
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include "ServerHeader.h"

using std::cout, std::endl;
using std::vector, std::string, std::ifstream, std::ofstream;

void HandleTCPClient(TCPSocket *sock); // TCP client handling function

const unsigned int RCVBUFSIZE = 100;    // Size of receive buffer

bool checkInVector(vector<string> vec, string toFind);

int getFileSize(string fileName);

// Stores user values
vector<vector<string>> uAttr(2, vector<string>());

int main(int argc, char *argv[]) {
    if (argc != 2) {                     // Test for correct number of arguments
        cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
        exit(1);
    }

    unsigned short mathServerPort = atoi(argv[1]);  // First arg: local port
    cout << "Server running on port " << mathServerPort << endl;

    cout << "Adding users: ";
    // Imports user values
    ifstream readUsers("users.txt");
    while (!readUsers.eof()) {
        vector<string> tempVals(2, "");
        for (int i = 0; i < 2; i++) readUsers >> tempVals.at(i);

        for (int i = 0; i < tempVals.size(); i++) {
            if (tempVals.at(i) != "") uAttr.at(i).push_back(tempVals.at(i));
        }
        cout << "*";
    }
    readUsers.close();
    cout << "\nUsers added, server started successfully.\n" << endl;

    try {
        TCPServerSocket serverSock(mathServerPort);     // Server Socket object

        while (true) {   // Run forever
            HandleTCPClient(serverSock.accept());       // Wait for a client to connect
        }
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return(0);
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {
    cout << "Handling client ";
    try {
        cout << sock->getForeignAddress() << ":";
    } catch (const std::exception& e) {
        cerr << "Unable to get foreign address" << endl;
        cerr << e.what() << endl;
    }
    try {
        cout << sock->getForeignPort();
    } catch (const std::exception& e) {
        cerr << "Unable to get foreign port" << endl;
        cerr << e.what() << endl;
    }
    cout << endl;


    // Initialization of necessary variables
    char username[64];
    char password[64];
    char userStringInput[2048];
    int conf = 1, deny = -1;
    int userInput;

    // Server receives username and password
    sock->recv(username, sizeof(username));
    sock->recv(password, sizeof(password));
    cout << "Received username '" << username << "' and password '" << password << "'." << endl;

    // If the username and password are valid, proceed
    if (checkInVector(uAttr.at(0), (string)username) && checkInVector(uAttr.at(1), (string)password)) {
        // Sends confirmation of username/password to client
        sock->send(&conf, sizeof(conf));

        // Receives user input based on client-side menu
        sock->recv(&userInput, sizeof(userInput));

        // If statement checks to make sure user input is valid
        if (userInput > 0 && userInput < 4) {
            // Sends confirmation of validation
            sock->send(&conf, sizeof(conf));

            // If the client wants to add a document
            if (userInput == 1) {
                // Server side message
                cout << "User is adding document." << endl;

                // Receives the document name and text from client
                char docBody[100000];   // Stores document text
                sock->recv(userStringInput, sizeof(userStringInput));
                sock->recv(docBody, sizeof(docBody));

                // Creates and writes to the document
                ofstream writeDoc(userStringInput);
                writeDoc << docBody;
                writeDoc.close();

                // Gets size of document to be stored in library.txt
                int fileSize = getFileSize(userStringInput);

                // Writes the document to the library
                if (receiveFile(fileSize, userStringInput, username) == 1) { sock->send(&conf, sizeof(deny)); }
                else { sock->send(&deny, sizeof(deny)); }

            }
            // If the client wants to view the library
            else if (userInput == 2) {
                cout << "User is viewing library." << endl;

                // Initialization of necessary variables
                char fileList[2048] = "\nList of files:\n";
                int docCount = 0;
                int totalSizeBytes = 0;

                // Saves a charstring file list inside fileList and saves the file names inside
                //   of fileVec
                vector<string> fileVec = printFiles(&docCount, &totalSizeBytes, fileList);

                // Sends the list of files and number of documents to the user to be printed
                sock->send(fileList, sizeof(fileList));
                sock->send(&docCount, sizeof(docCount));

                // Take in user input to determine next action
                sock->recv(&userInput, sizeof(userInput));

                // If client wants to view a document's contents
                if (userInput == 1) {
                    cout << "User is viewing a document." << endl;

                    // Receives client's choice of which document to view based on clientside menu
                    sock->recv(&userInput, sizeof(userInput));

                    // Opens requested document and prints server side message
                    ifstream readDoc(fileVec.at(userInput - 1));
                    cout << "User is viewing document '" << fileVec.at(userInput - 1)
                            << "'." << endl;
                    
                    // Saves contents of file inside 'page'
                    char page[100000];
                    int k = 0;
                    while(!readDoc.eof()) {
                        readDoc.get(page[k]);
                        k++;
                    }
                    readDoc.close();

                    // Page is sent to the client to be printed
                    sock->send(page, sizeof(page));

                }
                // If client wants to delete a document
                else if (userInput == 2) {
                    cout << "User is deleting a document." << endl;
                    
                    // Gets user input based on clientside menu
                    sock->recv(&userInput, sizeof(userInput));

                    // NOTE: Validity of userInput is checked on the client side.
                    //   If not valid, client sends -99, which signals that input is invalid
                    if (userInput != -99) {
                        // Only modifies library.txt if removing the file was successful
                        if (remove(fileVec.at(userInput - 1).c_str()) == 0) {
                            removeDoc(userInput, fileVec);

                            // Sends confirmation that document has been deleted to client
                            sock->send(&conf, sizeof(conf));
                        } else {
                            // If removing document was unsuccessful
                            cout << "Unable to delete document '" << fileVec.at(userInput - 1)
                                    << "'." << endl;
                            sock->send(&deny, sizeof(deny));
                        } 
                    } else {
                        cout << "User entered invalid file index." << endl;
                    }
                    
                }
                // User decides to exit
                else if (userInput == 3) {
                    // Exit message
                    cout << "Client " << sock->getForeignAddress() 
                            << ":" << sock->getForeignPort() << " exited." << endl;
                }
            }
            // User decides to exit
            else if (userInput == 3) {
                // Exit message
                cout << "Client " << sock->getForeignAddress() 
                        << ":" << sock->getForeignPort() << " exited." << endl;
            }
        } else {
            // Sends denial if user input is invalid
            sock->send(&deny, sizeof(deny));
        }

    } else {
        // Sends denial if username/password are invalid.
        sock->send(&deny, sizeof(deny));
    }

    delete sock;
}

// Checks if a value is in a vector
bool checkInVector(vector<string> vec, string toFind) {
    for (string val : vec) {
        if (val == toFind) return true;
    }
    return false;
}

// Gets the size of a file
int getFileSize(string fileName) {
    streampos begin,end;
    ifstream myfile (fileName, ios::binary);
    begin = myfile.tellg();
    myfile.seekg (0, ios::end);
    end = myfile.tellg();
    myfile.close();
    return end - begin;
}
