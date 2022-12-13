#include<iostream>
#include <pthread.h>
#include <sys/socket.h>
#include<cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sch_server.h>
#include <common.h>
using namespace std;

extern class TcpServer server;
struct process {
    string process_path;
    int arrival_time;
    int burst_time;
    int args;
    int start_time;
    int completion_time;
};

void TcpServer::initializeSocket() {

  _sockfd = socket(AF_INET , SOCK_STREAM , 0);

  if (_sockfd == -1) 
  {
    throw std::runtime_error(strerror(errno));
  }
}

void TcpServer::bindAddress(int port) {
    memset(&_serverAddress, 0, sizeof(_serverAddress));
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAddress.sin_port = htons(port);

    const int bindResult = bind(_sockfd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress));
    const bool bindFailed = (bindResult == -1);
    if (bindFailed) {
        throw std::runtime_error(strerror(errno));
    }
}


void TcpServer::listenToClients() {
    const int clientsQueueSize = _maxNumOfClients;
    const bool listenFailed = (listen(_sockfd, clientsQueueSize) == -1);
    if (listenFailed) {
        throw std::runtime_error(strerror(errno));
    }
}


void* handleClientMessages(void *arg)
{
  clientInfo_t *clInfo = (clientInfo_t *) arg;
  char msg[64];

  int ret = -1;

  
  while(1)
  {
    const size_t numOfBytesReceived = recv(clInfo->clSockFd,(void*) msg, sizeof(msg), 0);

    

    if(numOfBytesReceived < 1)
    {
      std::string errorMsg;
      if (numOfBytesReceived == 0)
      { //client closed connection
        errorMsg = "Client closed connection";
        close(clInfo->clSockFd);
        return NULL;
      } else {
        errorMsg = strerror(errno);
      }
      close(clInfo->clSockFd);
      return NULL;
    }

    	if(strcmp(msg,"startsc")==0)
	    {
		server.scheduler();
		
	    }
	    else if(strcmp(msg,"stopsch")==0)
	    {
		    server.stop();
	    }
	    else if(strcmp(msg,"ALGO_FC")==0)
	    {
		    server.fcfs();
		    
	    }
	    else if(strcmp(msg,"ALGO_RR")==0)
	    {
		    server.rr();
		    
	    }
	    else if(strcmp(msg,"ALGO_SJ")==0)
	    {
		    server.sjf();
		    
	    }
	    else if(strcmp(msg,"ALGO_PR")==0)
	    {
		    server.priority();
	    }
	    else
		    cout<<"Invalid msg"<<endl;

  }
  return NULL;
}

int TcpServer::acceptClient()
{
	//Accepting client
    pthread_t clientThread;
    socklen_t socketSize  = sizeof(_clientAddress);
    const int fileDescriptor = accept(_sockfd, (struct sockaddr*)&_clientAddress, &socketSize);

    const bool acceptFailed = (fileDescriptor == -1);
    if (acceptFailed) {
        throw std::runtime_error(strerror(errno));
    }

    clientInfo_t *newClientInfo = new (clientInfo_t);

    newClientInfo->clIpAddr = inet_ntoa(_clientAddress.sin_addr);
    newClientInfo->clSockFd = fileDescriptor;
    
    

    pthread_create(&clientThread, NULL, handleClientMessages, (void *) newClientInfo);

    _clThread.push_back(clientThread);

    return 1;
}

int TcpServer::close(int sockFd) 
{
  close(sockFd);
  return 1;
}

//scheduling using FCFS Algorithm

void TcpServer::fcfs()
{
	cout<<"FCFS Algorithm"<<endl;
	int n;
    	struct process p[100];
    	cout<<"Enter the number of processes: ";
    	cin>>n;

    	for(int i = 0; i < n; i++) {
		cout<<"Enter process path"<<i+1<<":";
		cin>>p[i].process_path;
        	cout<<"Enter arrival time of process "<<i+1<<": ";
        	cin>>p[i].arrival_time;
        	cout<<"Enter burst time of process "<<i+1<<": ";
        	cin>>p[i].burst_time;
               cout<<endl;
    	}

    


    	for(int i = 0; i < n; i++) {
        	p[i].start_time = (i == 0)?p[i].arrival_time:max(p[i-1].completion_time,p[i].arrival_time);
        	p[i].completion_time = p[i].start_time + p[i].burst_time;
    	}
    	cout<<endl;
    	cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"\n"<<endl;

    	for(int i = 0; i < n; i++) {
	    if(p[i].arrival_time==1)
	    {
		    cout<<"Current Running process is:"<<endl;
       		    cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\n"<<endl;
	    }
	    else if(p[i].arrival_time==2)
	    {
		    cout<<"Process ready to execute:"<<endl;
		    cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\n"<<endl;
	    }
	    else
	    {
		    cout<<"process under waiting state:"<<endl;
		    cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\n"<<endl;
	    }
    }
}

//scheduling using SJF algorithm
void TcpServer::sjf()
{
	cout<<"SJF Algorithm"<<endl;
	int n;
    	struct process p[100];
    	int is_completed[100];
    	memset(is_completed,0,sizeof(is_completed));
    
    	cout<<"Enter the number of processes: ";
    	cin>>n;

    	for(int i = 0; i < n; i++) {
        	cout<<"Enter path of the process"<<i+1<<": ";
        	cin>>p[i].process_path;
        	cout<<"Enter arrival time of process "<<i+1<<": ";
        	cin>>p[i].arrival_time;
        	cout<<"Enter burst time of process "<<i+1<<": ";
        	cin>>p[i].burst_time;
        	cout<<"\n";
        	cout<<endl;
    	}

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while(completed != n) {
        int idx = -1;
        int mn = 10000000;
        for(int i = 0; i < n; i++) {
            if(p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if(p[i].burst_time < mn) {
                    mn = p[i].burst_time;
                    idx = i;
                }
                if(p[i].burst_time == mn) {
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        mn = p[i].burst_time;
                        idx = i;
                    }
                }
            }
        }
        if(idx != -1) {
            p[idx].start_time = current_time;
            p[idx].completion_time = p[idx].start_time + p[idx].burst_time;

            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].completion_time;
            prev = current_time;
        }
        else {
            current_time++;
        }
        
    }

   
    cout<<endl<<endl;

    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"\n"<<endl;

    for(int i = 0; i < n; i++) {
        cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<"\t"<<"\n"<<endl;
    }
    
}
// scheduling using priority algorithm
void TcpServer::priority()
{
	cout<<"Priority based algorithm"<<endl;
	int n;
    	struct process p[100];
    
    	int is_completed[100];
    	memset(is_completed,0,sizeof(is_completed));

    	cout<<"Enter the number of processes: ";
    	cin>>n;

    	for(int i = 0; i < n; i++) {
        	cout<<"Enter process path "<<i+1<<": ";
        	cin>>p[i].process_path;
        	cout<<"Enter arrival time of process "<<i+1<<": ";
        	cin>>p[i].arrival_time;
        	cout<<"Enter burst time of process "<<i+1<<": ";
        	cin>>p[i].burst_time;
        	cout<<"Enter priority of the process "<<i+1<<": ";
        	cin>>p[i].args;
        	cout<<endl;
    	}

    	int current_time = 0;
    	int completed = 0;
    	int prev = 0;

    	while(completed != n) {
        	int idx = -1;
        	int mx = -1;
        	for(int i = 0; i < n; i++) {
            		if(p[i].arrival_time <= current_time && is_completed[i] == 0) {
                	if(p[i].args > mx) {
                    	mx = p[i].args;
                    	idx = i;
                }
                if(p[i].args == mx) {
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        mx = p[i].args;
                        idx = i;
                    }
                }
            }
        }
        if(idx != -1) {
            p[idx].start_time = current_time;
            p[idx].completion_time = p[idx].start_time + p[idx].burst_time;
            

            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].completion_time;
            prev = current_time;
        }
        else 
	{
            current_time++;
        }
        
    }

    cout<<endl<<endl;

    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"PRI\t"<<"ST\t"<<"CT\t"<<"\n"<<endl;

    for(int i = 0; i < n; i++) {
        cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].args<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<"\n"<<endl;
    }
}
void TcpServer::rr()
{
	cout<<"Round Robin Algorithm"<<endl;
	int n;
    	int tq;
    	struct process p[100];

    	int burst_remaining[100];
    	int idx;

    	cout<<"Enter the number of processes: ";
    	cin>>n;
    	cout<<"Enter time quantum: ";
    	cin>>tq;

    	for(int i = 0; i < n; i++) {
        	cout<<"Enter path of process "<<i+1<<": ";
        	cin>>p[i].process_path;
        	cout<<"Enter arrival time of process "<<i+1<<": ";
        	cin>>p[i].arrival_time;
        	cout<<"Enter burst time of process "<<i+1<<": ";
        	cin>>p[i].burst_time;
        	burst_remaining[i] = p[i].burst_time;

        	cout<<endl;
    }

    
    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark,0,sizeof(mark));
    mark[0] = 1;

    while(completed != n) {
        idx = q.front();
        q.pop();

        if(burst_remaining[idx] == p[idx].burst_time) {
            p[idx].start_time = max(current_time,p[idx].arrival_time);

        }

        if(burst_remaining[idx]-tq > 0) {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }
        else {
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].completion_time = current_time;

        }

        for(int i = 1; i < n; i++) {
            if(burst_remaining[i] > 0 && p[i].arrival_time <= current_time && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }
        if(burst_remaining[idx] > 0) {
            q.push(idx);
        }

        if(q.empty()) {
            for(int i = 1; i < n; i++) {
                if(burst_remaining[i] > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
}

    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"\n"<<endl;

    for(int i = 0; i < n; i++) {
        cout<<p[i].process_path<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<"\n"<<endl;
    }

}


