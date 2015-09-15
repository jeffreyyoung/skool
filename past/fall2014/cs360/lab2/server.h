#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>

#include <map>
#include <semaphore.h>
#include <queue>

#include "logger.h"


using namespace std;

struct DATA {
    vector<string> subjects;
    vector<string> messages;
    vector<int> sizes;
};

typedef map<string, DATA> MessageMap;
typedef pair<string, DATA> mapPair;


class Server {

public:
    Server();
    ~Server();

    void run(bool);
    
protected:
    virtual void create();
    virtual void close_socket();
    void serve();
    void handle(int);
    string get_request(int, string&);
    string get_request_message(int,int,string&);
    bool send_response(int, string);

    string parse_request(string, int, string&);
    void addToMap(string name, string subject, string message, int length);

    vector<string> split(string);

    void* runThread();

    static void* callMemberFunction(void *arg) { return ((Server*)arg)->runThread(); }

    int num_threads;
    queue<int> clients_queue;
    int server_;

    MessageMap map_;
    Logger logger_;

    sem_t *mailbox_sem;  //should these be global?
    sem_t *queue_sem;
    sem_t *n;
    sem_t *e;
};
