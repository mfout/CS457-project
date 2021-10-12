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
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[]){
  
  int opt;
  vector<char> flags;
  string port;
  string ip_address;
  
if(argc == 5 || argc == 2){ // Check to see if there is args. If there is not, then it means it is the server

    /****************************************************************
    *                         Client                                *
    ****************************************************************/

    while((opt = getopt(argc,argv,"hp:s:")) != -1){
        switch(opt){
                case 'p':{
                    for (char c : flags){
                        if(c == 'p'){
                            cerr << argv[0] << ": Error: -p was already called" << "\n";
                            exit(1);
                        }
                    }
                    flags.push_back('p');
                    port = optarg;
                    break;
                }
                case 's':{
                    for (char c : flags){
                        if(c == 's'){
                            cerr << argv[0] << ": Error: -s was already called" << "\n";
                            exit(1);
                        }
                    }
                    flags.push_back('s');
                    ip_address = optarg;
                    break;
                }
            case 'h':{
                cout << argv[0] << ": Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help |\n";
                exit(1);
                break;
            }
            default:
                cerr << argv[0] << ": Invalid Flag - Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help\n";
                break;
                exit(1);
            }
    }

    cout << ip_address << " " << port << "\n";

  }else if (argc == 0){
    /****************************************************************
    *                         Server                               *
    ****************************************************************/
 }else{
  // Bad Number of args
 }
  

struct Packet{
  uint16_t packetVersion;
  uint16_t packetLength;
  string message;
}

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

  send(processSocket, &mesage, sizeof(packet), 0);
}


void receiveMessage(){

}


void serverProcess(){
  while (true){ // Loop on inputs forever. 
    getMessage();
    sendMessage();
  }
}


void clientProcess(){
  while (true){
    sendMessage();
    getMessage();
  }
}return 0;
}
