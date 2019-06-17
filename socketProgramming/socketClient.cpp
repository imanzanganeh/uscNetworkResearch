#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
  int _socket = socket(AF_INET, SOCK_STREAM, 0);
  
}
