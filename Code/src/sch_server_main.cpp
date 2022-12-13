#include <iostream>
#include <vector>
#include <algorithm>
#include <sch_server.h>
#include <common.h>
using namespace std;
class TcpServer server;

int main() 
{
  int clNum = 0;
  int port = 65123;

  server.setMaxClients(1);

  server.initializeSocket();
  server.bindAddress(port);
  server.listenToClients();


  while(1)
  {
    int ret = server.acceptClient();

    if (ret) 
    {
      std::cout << "Client accepted successfully\n";
      clNum++;

      if (clNum == server.getMaxClients())
        break;
    }
  }

  vector<pthread_t> clThread = server.getClientThreads();
 
  for (auto i = clThread.begin(); i != clThread.end(); ++i)
  {
    server.waitForClThread(*i);
  }


  return 0;
}

