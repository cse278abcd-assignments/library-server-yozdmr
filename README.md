**Introductory description:**  
My client-server application is a library management system. My four functionalities are: logging into the server, adding documents to the library, viewing a document in the library, and removing a document from the library.

**User login system:**  
	The server stores users and their passwords in a file called "users.txt". Each line has the username and password of a user.
	EX: 'ExampleUser password9518'
	When a client first connects to the server, they are prompted to enter their username and password. If they enter existing login information, they are allowed access to the server. Otherwise, they are denied access and their connection terminates. 
	There is no way to modify the users (add, change, delete) inside of the program. One must edit the users.txt file directly, maintaining proper formatting.

**Storage system:**  
	The library stores the names of all documents it contains inside a file called "library.txt". Each line has the name of a document, its size in bytes, and the author (person who added the document to the library).
	EX: 'exampledoc.txt 52 ExampleUser'

**Adding a document:**  
	When a client requests the addition of a new document, they will first be promted for the name of the document. After they enter that, they will be able to enter the contents of the document line by line until they are finished. The client can exit by entering '::q".
	It is important to note that a client cannot modify a line once it has been entered. Therefore it is important to make sure your spelling is correct.
	Once the client is finished writing the document, the document name and username of the current client will be sent to the server. The server will receive these and add the document to the database in "library.txt". The client-side program will then exit.

**Viewing a document:**
	When a client requests to view a document, they will be prompted with a list of documents in the database. They can then choose which document they want to view. Upon choosing, the contents of the document will be printed to them. The program will then exit.

**Deleting a document:**
	When a client requests the deletion of a document, they will be prompted with a list of documents in the database. They can then choose which document they want to delete. They can then delete the document permanently if they wish. If the server is for some reason unable to delete the document, the database will not be modified, and the user will be disconnected with a message.

**Using the makefile**
	COMING SOON
  
  
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=9090871&assignment_repo_type=AssignmentRepo)
