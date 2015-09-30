#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include <vector>
#include <sstream>

#include "logger.h"

using namespace std;

class Client {
public:
    Client();
    ~Client();

    void run(bool);

protected:
    virtual void create();
    virtual void close_socket();
    void echo();
    bool send_request(string);
    string get_response();

    vector<string> split(string);
    vector<string> split_by_line(string);
    string handle_command(string);
    string get_user_message();
    string handle_response(string, string);

    string fail_;
    int server_;
    int buflen_;
    char* buf_;
    Logger logger_;
};
