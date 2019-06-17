#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

int main()
{
   //Create a socket
  int _socket = socket(AF_INET, SOCK_STREAM, 0);//end connection port for server

  //Bind the socket to an IP / port
  sockaddr_in handle;//sockaddr_in is a struct that contains an internet address. It is used for sockets
 
  handle.sin_port = htons(54000);//htons changes the host byte order itno network byte order. Essentially, 54000 gets changed to network ip address. 54000 represents a private port

  if (bind(_socket, (sockaddr*)&handle, sizeof(handle)) == -1)//binds identification information to the server socket
    {
      cerr << "Error.  Can't bind to IP port";
      return -1;
    }

  //Mark socket listening in
   if (listen(_socket, SOMAXCONN) == -1)
    {
      cerr << "Error. Server can't listen";
      return -1;
    }
  
  //Accepting a call from a client
  sockaddr_in _client;//Internet address of client; in network byte order/big endian
  socklen_t clientSize = sizeof(_client);
  char host[NI_MAXHOST];//Maximum size of host length buffer(buffer means storage)
  char svc[NI_MAXSERV];//Maximum size of server length buffer
  
  int clientSocket = accept(_socket, (sockaddr*) &_client, &clientSize);//clientSocket is accepting the thread made to the server socket

  int result = getnameinfo((sockaddr*)&_client, clientSize, host, NI_MAXHOST, svc, NI_MAXSERV, 0);//Socket address structure to hostname and service name. Produces host name, which is the name of the computer.  If equal to 0, then function has failed

  //if-else statement is actual connection process I think(check and make sure and verify)
   if (result != 0)
    {
      cout << host << " Connected on " << svc << endl;
    }
   else
    {
      inet_ntop(AF_INET, &_client.sin_addr,host, NI_MAXHOST);//converts from network format to presentation format such that it can be displayed. Presentation format will be a string. Same as inet_ntoa except it extends to being compatible with multiple address families. Essentially it converts the Internet host address in, given in network byte order(big endian byte order, which is essentiall where numbers in ip address are decreasing, or in other words, highest byte first), to a string in IPv4 dotted-decimal notation(ddd.ddd.ddd.ddd).
      cout << host << "connected on" << ntohs(_client.sin_port) << endl;
    }
  
  char buf[4096];//buffer
  while (true)
    {
      memset(buf, 0, 4096);//Resets buffer with 0's for every iteration of the while loop such that current input doesn't get concatinated with previous input  
      int bytesReceiver = recv(clientSocket, buf, 4096, 0);//receive a message from a socket

      if (bytesReceiver == -1)//for if user terminates window in which telnet is being used
	{
	  cerr << "There was a connection issue" << endl;
	  break;
	}

       if (bytesReceiver == 0)
	{
	  cout << "The client disconnected" << endl;
	  break;
	}
      
      //display message
      cout << "received: " << string(buf, 0, bytesReceiver) << endl;

      //resend message
      send(clientSocket, buf, bytesReceiver + 1, 0);
    }

  return 0;
}
