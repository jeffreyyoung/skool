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
    string get_request(int);
    string get_request_message(int,int);
    bool send_response(int, string);

    string parse_request(string, int);
    void addToMap(string name, string subject, string message, int length);
    string get_data(vector<string> lines);

    vector<string> split(string);

    int server_;
    int buflen_;
    char* buf_;
    MessageMap map_;
    Logger logger_;
    string cache_;
};
