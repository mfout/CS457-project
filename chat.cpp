/**************************************
 * Group Name  : Frame                *
 *                                    *
 * Member1 Name: Caleb Cluett         *
 * Member1 SIS ID: 831209541          *
 * Member1 Login ID: ccluett          *
 *                                    *
 * Member2 Name: Mason Fout           *
 * Member2 SIS ID: 831378374          *
 * Member2 Login ID: mfout            *
 **************************************/

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
#include <stdio.h>
#include <cstring>
#include <regex>

using namespace std;

void sendMessage(int processSocket);
void receiveMessage(int processSocket);
void serverProcess(int serverSocket);
void clientProcess(int clientSocket);
void validPort(const string &port);
void validIP(const string &host);

int main(int argc, char* argv[]){
  
  int opt;
  vector<char> flags;
  string port;
  string ip_address;
  
  

  if (argc == 1){  

    /*********************************************
     *                 Server                    *
     *********************************************/ 
    
    struct addrinfo *self;
    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *result;
    
    int success = getaddrinfo(NULL, "3333", &hints, &result);
    if (success != 0){
      cerr << "server failure";
      exit(1);
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
      exit(1);
    }

    int bindSuccess = bind(newSocket, self->ai_addr, self->ai_addrlen);

    if (bindSuccess == -1){
      cerr << "Binding failed";
      exit(1);
    }

    int listenSuccess = listen(newSocket, 20);
    if (listenSuccess == -1){
      cerr << "socket failed to listen";
      exit(1);
    }

    struct sockaddr clientAddr{};
    socklen_t addressLength;
    int serverSocket = accept(newSocket, &clientAddr, &addressLength);

    if (serverSocket == -1){
      cerr << "Failed to Accept";
      exit(1);
    }

    cout << "Found a friend! You receive first.\n";

    serverProcess(serverSocket);
  }
  
  else if (argc == 5 || argc == 2 || argc == 3){ 

    /*********************************************
     *                 Client                    *
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

    validPort(port);
    validIP(ip_address);
  
    struct addrinfo hints{};
    struct addrinfo *server;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip_address.c_str(), port.c_str(), &hints, &server);
    // Bind the socket to the IP and Port
    int newSocket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (newSocket == -1){
      cerr << "Socket could not be created";
      exit(1);
    }

    cout << "Welcome to Chat!\nConnecting to server...\n";

    int newConnect = connect(newSocket, server->ai_addr, server->ai_addrlen);
    if(newConnect == -1){
      cerr << "Could not connect to server\n";
      exit(1);
    }

    cout << "Connected!\nConnected to a friend! Your send first.\n";

    clientProcess(newSocket);
  }
}

struct Packet{
  uint16_t packetVersion;
  uint16_t packetLength;
  char message[140];
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
  //packet.message = message;

  memcpy(packet.message, message.c_str(), message.length());

  send(processSocket, &packet, sizeof(packet), 0);
}


void receiveMessage(int processSocket){
  Packet packet{};
  int receivedBytes = recv(processSocket, &packet, sizeof(packet), 0);
  if (receivedBytes == 0){
    cout << "Your friend left. Disconnecting.\n";
    close(processSocket);
    cerr << "Friend has left the chat. \n";
  }
  packet.packetLength = ntohs(packet.packetLength);

  cout << "Friend: " << string(packet.message,packet.packetLength) << endl;
}


void serverProcess(int serverSocket){
  while (true){
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

void validIP(const string &host) {
    regex pattern(R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))");
    smatch IP;

    bool validIP = regex_match(host, IP, pattern);

    if (!validIP) {
      cerr << "Invalid IP address.\n";
      exit(1);
    }

    for (int i = 1; i <= 4; i++) {
        int octet = stoi(IP[i]);
        if (octet > 255 || octet < 0) {
          cerr << "Invalid IP address\n";
          exit(1);
        }
    }
}

void validPort(const string &port) {
    int testPort = stoi(port);
    if (testPort > 65535 || testPort < 0){
         cerr << "Invalid port\n";
         exit(1);
    }
}
