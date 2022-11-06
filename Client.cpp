/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
*/
// Author Yusuf Ozdemir 2022

#include <iostream>
#include <cstring>
#include <fstream>
#include "PracticalSocket.h"  // For Socket

using std::cout, std::cin, std::cerr, std::endl;
using std::string, std::ifstream, std::ofstream;

const int RCVBUFSIZE = 30;    // Size of receive buffer

void mainMenu();
void libraryMenu();

int main(int argc, char *argv[]) {
    if (argc != 3) {     // Test for correct number of arguments
        cerr << "Usage: " << argv[0]
             << " <Server> <Port>" << endl;
        exit(1);
    }

    string serverAddress = argv[1]; // First arg: server address
    unsigned short mathServerPort = atoi(argv[2]);

    // Initialization of variables
    char username[64];
    char password[64];
    char userStringInput[2048];
    int errCode = -99;
    int codeVal;
    int userInput;

    try {
        // Establish connection with the echo server
        TCPSocket sock(serverAddress, mathServerPort);

        // Prompt user to enter username and password to log into server
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Sends username and password
        sock.send(username, sizeof(username));
        sock.send(password, sizeof(password));

        // Receive the confirmation of username and password
        sock.recv(&codeVal, sizeof(codeVal));

        if (codeVal == 1) {
            cout << "Login confirmed by server. Welcome!\n" << endl;

            mainMenu();  // Menu that provides initial options
            cin >> userInput;
            sock.send(&userInput, sizeof(userInput));  // Sends userInput to server
            
            sock.recv(&codeVal, sizeof(codeVal));  // Receives whether input was valid
            if (codeVal == 1) {
                // If the client wants to create a document
                if (userInput == 1) {
                    
                    // Gets name of new document
                    cout << "Name of document: ";
                    cin >> userStringInput;

                    // Instructions for creating a document
                    cout << "Write what will be inside the document. Enter ':q' to quit:" << endl;
                    
                    // While loop takes line input and adds it to docBody. The loop
                    //   ends if the user enters ':q'
                    char docBody[100000] = "";
                    string input;
                    while (input != ":q") {
                        getline(cin, input);
                        if (input != ":q") {
                            input += "\n";
                            strcat(docBody, input.c_str());
                        }
                    }

                    // Sends doc name and body to server
                    sock.send(userStringInput, sizeof(userStringInput));
                    sock.send(docBody, sizeof(docBody));
                    cout << "File sent." << endl;

                    // Receives code as to whether file was received by server
                    //   Prints message accordingly
                    sock.recv(&codeVal, sizeof(codeVal));
                    if (codeVal == 1) {
                        cout << "Server has received and stored the file." << endl;
                    } else {
                        cout << "Server was unable to receive the file." << endl;
                    }

                }
                // If the client wants to view library options
                else if (userInput == 2) {

                    // User receives cstring that will be printed to display files
                    //  that are currently in library, along with numFiles for future use
                    char fileList[2048];
                    int numFiles;
                    sock.recv(fileList, sizeof(fileList));
                    sock.recv(&numFiles, sizeof(numFiles));
                    cout << fileList << endl;

                    // Asks the user whether they want to view or delete a document
                    libraryMenu();  // Prints options for library
                    cin >> userInput;
                    sock.send(&userInput, sizeof(userInput));

                    // If the client wants to view a document
                    if (userInput == 1) {
                        
                        // Takes input from user
                        cout << "Which file do you want to view?\nInput: ";
                        cin >> userInput;

                        // Checks to make sure that input is in proper range
                        if (userInput < 1 || userInput > numFiles) {
                            cerr << "That number is not associated with any file. Refer to the list above." << endl;
                            exit(1);
                        }

                        // Sends the user input to server, server returns content
                        //  of doc in a cstring, which is printed
                        sock.send(&userInput, sizeof(userInput));
                        char docContents[100000];
                        sock.recv(docContents, sizeof(docContents));
                        cout << docContents << endl;  // Document is printed

                    }
                    // If the client wants to delete a document
                    else if (userInput == 2) {
                        // Gets user input
                        cout << "Which file do you want to delete?\nInput: ";
                        cin >> userInput;

                        // Checks to make sure that input is in proper range
                        if (userInput < 1 || userInput > numFiles) {
                            cerr << "That number is not associated with any file. Refer to the list above." << endl;
                            // If not, client sends -99 which signifies error
                            sock.send(&errCode, sizeof(errCode));
                            exit(1);
                        } else {
                            // If okay, user input is sent
                            sock.send(&userInput, sizeof(userInput));

                            // Client receives confirmation of file deletion
                            sock.recv(&codeVal, sizeof(userInput));
                            if (codeVal == 1) {
                                cout << "File successfully deleted." << endl;
                            } else {
                                cout << "Server was unable to delete file." << endl;
                            }
                        }

                    }
                    // If the client exits the program
                    else if (userInput == 3) {
                        cout << "Program exited." << endl;
                        exit(0);
                    }
                    // If the client enters an invalid input
                    else {
                        cout << "Invalid input." << endl;
                        exit(1);
                    }

                }
                // If the client exits the program
                else if (userInput == 3) {
                    cout << "Program exited." << endl;
                    exit(0);
                }
                // If the client enters an invalid input
                else {
                    cerr << "Invalid input." << endl;
                    exit(1);
                }
            }
            // If the client enters an invalid input
            else {
                cerr << "Server denied request. Maybe you entered an invalid input?" << endl;
                exit(1);
            }  

        }
        // If the client's login access is denied
        else {
            cerr << "Access denied. Perhaps you entered your username/password incorrectly." << endl;
            exit(1);
        }

    } catch(std::exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return 0;
}

void mainMenu() {
    cout << "What would you like to do?\n";
    cout << "1. Add document\n2. View library\n3. Exit";
    cout << "\nInput: ";
}

void libraryMenu() {
    cout << "What would you like to do?\n";
    cout << "1. View document\n2. Delete document\n3. Exit";
    cout << "\nInput: ";
}
