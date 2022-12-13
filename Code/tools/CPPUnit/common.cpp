#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>          
#include <sys/stat.h>        
#include <sch_client.h>
#include "common.h"
#include "sch_server.h"

#define SELECT_FAILED -1
#define SELECT_TIMEOUT 0

using namespace std;

namespace fd_wait {
    
     //monitor file descriptor and wait for I/O operation
     
    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds) {
        struct timeval tv;
        tv.tv_sec = timeoutSeconds;
        tv.tv_usec = 0;
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(fileDescriptor, &fds);
        const int selectRet = select(fileDescriptor + 1, &fds, nullptr, nullptr, &tv);

        if (selectRet == SELECT_FAILED) {
            return Result::FAILURE;
        } else if (selectRet == SELECT_TIMEOUT) {
            return Result::TIMEOUT;
        }
        return Result::SUCCESS;
    }
}

void Messaging::initialize_messages ()
{
  messages_t m;
  cout << endl;
}

void Messaging::create_message (msg_types_e option,int n, string process_path, int arrival_time, int burst_time, int arg)
{
 

  char sendBuffer[100];
  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {
    if (option  == ALGO_FCFS)
    {
	msg_map[option].process_path=process_path;
	msg_map[option].arrival_time=arrival_time;
	msg_map[option].burst_time=burst_time;
	msg_map[option].arg=arg;


	snprintf(sendBuffer, sizeof(sendBuffer), "%d%s%d%d%d\r\n", msg_map[option].msg_type, msg_map[option].process_path.c_str(), msg_map[option].arrival_time, msg_map[option].burst_time,msg_map[option].arg);
	

    }

    else if(option== ALGO_RR)
    {
	        	      
      msg_map[option].process_path = process_path;
      msg_map[option].arrival_time = arrival_time;
      msg_map[option].burst_time   = burst_time;
      msg_map[option].arg          =arg;

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%s%d%d%d\r\n", msg_map[option].msg_type, msg_map[option].process_path.c_str(), msg_map[option].arrival_time, msg_map[option].burst_time,msg_map[option].arg);
    }
    else if( option==ALGO_SJF)
    {
      msg_map[option].process_path = process_path;
      msg_map[option].arrival_time = arrival_time;
      msg_map[option].burst_time   = burst_time;
      msg_map[option].arg          =arg;

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%s%d%d%d\r\n", msg_map[option].msg_type, msg_map[option].process_path.c_str(), msg_map[option].arrival_time, msg_map[option].burst_time,msg_map[option].arg);
	map<msg_types_e,messages_t>::iterator itr;
	for(itr=msg_map.begin();itr!=msg_map.end();++itr)
	{
		cout<<'\t'<<itr->first<<'\t'<<itr->second.msg_type<<'\t'<<itr->second.process_path<<'\t'<<itr->second.arrival_time<<'\t'<<itr->second.arg<<'\n';
	
	}

	cout<<endl;

    }
       else if( option== ALGO_PRIORITY)
    {
      msg_map[option].process_path = process_path;
      msg_map[option].arrival_time = arrival_time;
      msg_map[option].burst_time   = burst_time;
      msg_map[option].arg          =arg;

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%s%d%d%d\r\n", msg_map[option].msg_type, msg_map[option].process_path.c_str(), msg_map[option].arrival_time, msg_map[option].burst_time,msg_map[option].arg);
    }
    else
      snprintf(sendBuffer, sizeof(sendBuffer), "%d%s%d%d%d\r\n", msg_map[option].msg_type, msg_map[option].process_path.c_str(), msg_map[option].arrival_time, msg_map[option].burst_time, msg_map[option].arg);
  }
 
}


void Messaging::create_message (msg_types_e option)
{
    

  	char sendBuffer[128];
  	string s = "";

  	memset(sendBuffer, '0', sizeof(sendBuffer));

  	if(msg_map.find(option)!= msg_map.end())
  	{
    		snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type); 
  	}

 
}


