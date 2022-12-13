#include <iostream>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#include "common.h"
#include <string.h>
#include "sch_client.h"

using namespace std;

TcpClient client;
Messaging msg;
void printMenu() {
    cout << "Select one of the following options: \n" <<
                 "1. Start the scheduler \n" <<
                 "2. Stop the scheduler  \n"  <<
                 "3. FCFS \n"<<
    		 "4. Round Robin \n"<<
		 "5. SJF \n"<<
		 "6. Priority \n";
}

int getMenuSelection() {
    int selection = 0;
    cin >> selection;
    if (!std::cin) {
        throw std::runtime_error("invalid menu input. expected a number, but got something else");
    }
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    return selection;
}
bool handleMenuSelection(int selection) {
		string tmp="";
    	static const int minSelection = START_SCHEDULER;
    	static const int maxSelection = ALGO_PRIORITY + 1;
    	if (selection < minSelection || selection > maxSelection) {
        cout << "Invalid selection: " << selection <<
                     ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
        return false;
    	}

    	switch (selection) {
        case START_SCHEDULER: {
            cout << "Sending message to start scheduler \n";
	    tmp.append("startsc");
            msg.create_message(START_SCHEDULER);
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
	    break;
        }
	case STOP_SCHEDULER: { 
	    msg.create_message(STOP_SCHEDULER);
	    tmp.append("stopsch");
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
            break;
        }
	case ALGO_FCFS: {
	    msg.create_message(ALGO_FCFS);
	    tmp.append("ALGO_FC");
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
	    break;
	}
	case ALGO_RR:{
	    msg.create_message(ALGO_RR);
	    tmp.append("ALGO_RR");
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
	    break;
	}
 	case ALGO_SJF:{
	    msg.create_message(ALGO_SJF);
	    tmp.append("ALGO_SJ");
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
	    break;
	}
	case ALGO_PRIORITY:{
	    msg.create_message(ALGO_PRIORITY);
	    tmp.append("ALGO_PR");
	    client.sendMsg(tmp.c_str(),sizeof(tmp.c_str()));
	    break;
	}

	default: {
            cout << "Invalid selection, " << selection <<" selection must be b/w " << minSelection << " and " << maxSelection << "\n";
        }
}

	client.sendMsg(tmp.c_str(),strlen(tmp.c_str()));	


    return false;
}

#define SCHEDULER_IP    "127.0.0.1"
#define SCHEDULER_PORT  "65123"
int main() 
{

  
  string ip = "127.0.0.1";
  int port = 65123;

  bool connected = false;
  int x = 0;

  while (!connected) 
  {
    connected = client.connectTo(ip, port);

    if (connected) 
    {
      std::cout << "Client connected successfully\n";
    } else 
    {
      std::cout << "Client failed to connect: " << "\n"
        << "Make sure the server is open and listening\n\n";
      sleep(2);
      std::cout << "Retrying to connect...\n";
      x++;
    }
      if (x == 2)
      {
        std::cout << "Exceeded max retries\n" << endl;
        exit(0);
      }
    }
   bool shouldTerminate = false;
   while(!shouldTerminate)
   {
    	printMenu();
    	int selection = getMenuSelection();
    	cout << "selection" << selection << endl;
    	shouldTerminate = handleMenuSelection(selection);
   }

    return 0;
}





