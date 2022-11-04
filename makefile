# Compiles the client.
client: PracticalSocket.o #lib.o
		g++ -Wall -std=gnu++17 -std=c++17 -o Client Client.cpp PracticalSocket.o

# Compiles the server.
server: PracticalSocket.o ServerHeader.o
		g++ -Wall -std=gnu++17 -std=c++17 -o Server Server.cpp PracticalSocket.o ServerHeader.o



# Compiles the PracticalSocket header.
PracticalSocket.o: PracticalSocket.cpp PracticalSocket.h
		g++ -Wall -std=gnu++17 -std=c++17 -c PracticalSocket.cpp

ServerHeader.o: ServerHeader.cpp ServerHeader.h
		g++ -std=gnu++17 -std=c++17 -c ServerHeader.cpp



# Just runs the client.
run-client:
		./Client localhost 9090

# Compiles and runs the client.
c-run-client: client
		./Client localhost 9090

# Just runs the server.
run-server:
		./Server 9090

# Compiles and runs the server.
c-run-server: server
		./Server 9090


# Misc
clean:
		rm *.o
		rm Client
		rm Server

make-files:
		touch users.txt
		touch librar.txt