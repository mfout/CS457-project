/*******************************************
* Group Name  : XXXXXX
* Member1 Name: XXXXXX
* Member1 SIS ID: XXXXXX
* Member1 Login ID: XXXXXX
* Member2 Name: XXXXXX
* Member2 SIS ID: XXXXXX
* Member2 Login ID: XXXXXX
********************************************/

// https://www.geeksforgeeks.org/socket-programming-cc/ (USEFUL RESOURCE)

#include <unistd.h>
#include <utility>      
#include <string>  
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

void sendMessage(int processSocket);
void receiveMessage(int processSocket);
void serverProcess(int serverSocket);
void clientProcess(int clientSocket);

int main(int argc, char* argv[]){
  
  int opt;
  vector<char> flags;
  string port;
  string ip_address;
  
  

  if (argc == 1){ // No args passed, this is the server
    // Find the IP, pick a Port, output both. 
    // Create a socket with that info. 

    /*********************************************
    *                 Server                     *
    *********************************************/ 

    
    struct addrinfo *self;
    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *result;
    
    int success = getaddrinfo(NULL, "3333", &hints, &result);//What port do you want to use?
    if (success != 0){
      cerr << "server failure";
    }

    self = result;
    char host[256];
    string ipAddress;

    gethostname(host, 256);
    struct hostent *hostIP = gethostbyname(host);
    ipAddress = inet_ntoa(*((struct in_addr*) hostIP->h_addr_list[0]));
  
    cout << "Welcome to chat!\nWaiting for a connection on ";
    cout << ipAddress;
    cout << " port 3333" << endl;

    

    int newSocket = socket(self->ai_family, self->ai_socktype, self->ai_protocol);
    if (newSocket == -1){
      cerr << "Socket could not be created";
    }

    int listenSuccess = listen(newSocket, 20);
    if (listenSuccess == -1){
      cerr << "socket failed to listen";
    }

    struct sockaddr clientAddr{};
    socklen_t addressLength;
    int serverSocket = accept(newSocket, &clientAddr, &addressLength);


    cout << "Found a friend! You receive first.\n";

    serverProcess(serverSocket);
  }
  
  else if (argc == 5 || argc == 2){ 

    /*********************************************
    *                 Client                     *
    *********************************************/ 

    while((opt = getopt(argc, argv, "hp:s:")) != -1){
      switch(opt){
        case 'p' : {
          for (char c : flags){
            if(c == 'p'){
              cerr << argv[0] << ": Error: -p was already called" << "\n";
              exit(1);
            }
          }
          flags.push_back('p');
          port = optarg;
          break;
        }case 's' : {
          for (char c : flags){
            if(c == 's'){
              cerr << argv[0] << ": Error: -s was already called" << "\n";
              exit(1);
            }
          }
          flags.push_back('s');
          ip_address = optarg;
          break;
        }case 'h' : {
          cout << argv[0] << ": Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help";
          exit(1);
          break;
        }default : {
          cerr << argv[0] << ": Invalid Flag - Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help";
        }
      }
    }

    struct addrinfo hints{};
    struct addrinfo *server;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip_address.c_str(), port.c_str(), &hints, &server);
    // Bind the socket to the IP and Port
    int newSocket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (newSocket == -1){
      cerr << "Socket could not be created";
    }
    clientProcess(newSocket);
  }
}

struct Packet{
  uint16_t packetVersion;
  uint16_t packetLength;
  string message;
};


void sendMessage(int processSocket){
  string message;
  do{
    cout << "You: ";
    getline(cin, message);
    if (message.length() > 140){
      cout << "Message limited to 140 characters.\n";
    }
  }while(message.length() > 140);

  // Send via socket
  Packet packet{};
  packet.packetVersion = htons(457);
  packet.packetLength = htons(message.length());
  packet.message = message;

  send(processSocket, &message, sizeof(packet), 0);
}


void receiveMessage(int processSocket){
  Packet packet{};
  int receivedBytes = recv(processSocket, &packet, sizeof(packet), 0);
  if (receivedBytes == 0){
    cout << "Your friend left. Disconnecting.\n";
    close(processSocket);
    cerr << "Friend has left the chat.";
  }
  packet.packetLength = ntohs(packet.packetLength);

  cout << "Friend: " << string(packet.message, packet.packetLength) << "\n";
}


void serverProcess(int serverSocket){
  while (true){ // Loop on inputs forever. 
    receiveMessage(serverSocket);
    sendMessage(serverSocket);
  }
}


void clientProcess(int clientSocket){
  while (true){
    sendMessage(clientSocket);
    receiveMessage(clientSocket);
  }
}
