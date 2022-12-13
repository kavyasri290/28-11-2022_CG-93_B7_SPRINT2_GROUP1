#pragma once
#ifndef COMMON_H
#define COMMON_H
#include <cstdio>
#include <string>
#include <mqueue.h>
#include <bits/stdc++.h>

using namespace std;
#define MAX_PACKET_SIZE 4096
namespace fd_wait {
    enum Result {
        FAILURE,
        TIMEOUT,
        SUCCESS
    };

    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds = 1);
};

typedef enum 
{
  START_SCHEDULER = 1,
  STOP_SCHEDULER  = 2,
  ALGO_FCFS       = 3,
  ALGO_RR         = 4,
  ALGO_SJF        = 5, 
  ALGO_PRIORITY   = 6,

} msg_types_e;

typedef struct messages
{
  msg_types_e msg_type;
  string process_path;
  int arrival_time;
  int burst_time;
  int arg;
  
}messages_t;

class Messaging
{
  map<msg_types_e, messages_t>  msg_map;

  public:
    Messaging()
    {
      initialize_messages();
    }
    void create_message(msg_types_e option, int n,string process_path, int arrival_time,int burst_time,int arg);
    void  create_message(msg_types_e option);
    void initialize_messages();
};

#endif
