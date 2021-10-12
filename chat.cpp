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

using namespace std;

int main(int argc, char* argv[]){
  
  int opt;
  vector<char> flags;
  int port;
  string ip_address;
  
  while((opt = getopt(argc, argv, "+hp:s:")) != 1){
  	switch(opt){
      		case 'p':
        		for (char c : flags){
				if(c == 'p'){
					cerr << argv[0] << ": Error: -p was already called" << "\n";
					exit(1);
		 		}
       			}
       			flags.push_back('p');
        		port = optarg;
        		break;
      		case 's':
        		for (char c : flags){
				if(c == 's'){
					cerr << argv[0] << ": Error: -s was already called" << "\n";
					exit(1);
				}
        		}
        		flags.push_back('s');
        		ip_address = optarg;
        		break;
			
		case 'h':
			cout << argv[0] << ": Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help";
			exit(1);
			break;
		default:
			cerr < argv[0] << ": Invalid Flag - Usage: -s '[IP Address]' | -p '[Port Number]' | -h Help";
	
    	}
  }

  if (argc == 1){ // No args passed, this is the server
    // Find the IP, pick a Port, output both. 
    // Create a socket with that info. 

    while (true){ // Loop on inputs forever. 
      // Wait for message
      // Get port/IP from message (or bind to socket?)
      // Get user input
      // Send via port/IP
    }
  }
  else if (argc == 3){ // Args were passed, this is the client
    // Port and IP should be arguements, take from those. 
      // Check flags to find arguements.
      // End if the args are incorrect

    // Bind the socket to the IP and Port

    while (true){
      // Get user input
      // Send through socket
      // Wait for reply
    }
  }
  
  
  // Incorrect number of args
  return 0;
}
