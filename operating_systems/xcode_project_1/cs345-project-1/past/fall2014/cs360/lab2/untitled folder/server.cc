#include "server.h"

Server::Server() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
    num_threads_ = 10;
}

Server::~Server() {
    delete buf_;
}

void
Server::run(bool debug) {
    // create and run the server
    logger_.set_is_activated(debug);
    create();
    serve();
}

void
Server::create() {
}

void
Server::close_socket() {
}

void
Server::serve() {
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

    //create 10 threads
    //make a queue
    //
    sem_t mailbox_sem;  //should these be global?
    sem_t queue_sem;
    sem_t n;
    sem_t e;

    sem_init(&mailbox_sem, 0 , 1);
    sem_init(&queue_sem, 0, 1);
    sem_init(&n, 0, 0);
    sem_init(&e, 0, num_threads_);

    //create ten threads


      // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {

        //e semaphore
        //push client to queue
        sem_wait(&e);
        handle(client);
        sem_post(&e);
    }
    close_socket();
}

void*
Server:: run_thread(){

    handle(client);

}

vector<string>
Server::split(string str){
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf)
        tokens.push_back(buf);

    return tokens;
}

void 
Server:: addToMap(string name, string subject, string message, int size){
    logger_.log("adding to map for " + name);
    map<string, DATA>::iterator it = map_.find(name);
    logger_.log("adding: " + message);
    if (it != map_.end()){
        //add to existing
        map_[name].subjects.push_back(subject);
        map_[name].messages.push_back(message);
        map_[name].sizes.push_back(size);
    }
    else {
        DATA data;
        data.subjects.push_back(subject);
        data.messages.push_back(message);
        data.sizes.push_back(size);
        map_.insert(mapPair(name, data));
        //create new
    }
}

string
Server::parse_request(string request, int client){
    logger_.log("parsing request: " + request);
    vector<string> fields = split(request);

    for (int i = 0; i < fields.size(); i++){
        logger_.log(to_string(i) + " " + fields.at(i));
    }

    if (fields.size() == 0){
        return "error invalid message\n";
    }
    else if (fields.at(0) == "reset"){
        map_.clear();
        //reset message
        return "OK\n";
    }
    else if (fields.at(0) == "put" && fields.size() >= 4) {
        string name = fields.at(1);
        string subject = fields.at(2);
        int length = atoi(fields.at(3).c_str());
        logger_.log("about to get data...");
        string data = get_request_message(client, length);
        addToMap(name, subject, data, length);
        return "OK\n";
    }
    else if (fields.at(0) == "list" && fields.size() ==2){
        string name = fields.at(1);

        map<string, DATA>::iterator it = map_.find(name);

        if (it != map_.end()){
            logger_.log("listing data");

            string str = "list " + to_string(map_[name].subjects.size()) + "\n";
            cout << str << endl;
            for (int i = 0; i < map_[name].subjects.size(); i++){
                str += to_string(i + 1) + " " + map_[name].subjects.at(i) + "\n";
            }
            return str;
        }
        else {
            return "error user not found\n";
        }

        return "list\n";
    }
    else if (fields.at(0) == "get" && fields.size() == 3){
        string name = fields.at(1);
        int index = atoi(fields.at(2).c_str());
        index--;
        map<string, DATA>::iterator it = map_.find(name);

        if (it != map_.end()){
            logger_.log("getting data");
            if (index >= map_[name].subjects.size()){
                return "error index too large\n";
            }

            string subject = map_[name].subjects.at(index);
            string message = map_[name].messages.at(index);
            int length = map_[name].sizes.at(index);

            string response = "message " + subject + " " + to_string(length) + "\n" + message;

            return response;
        }
        else {
            return "error invalid message\n";
        }
    }
    else {
        return "error invalid message\n";
    }
}

void
Server::handle(int client) {
    // loop to handle all requests
    while (1) {
        // get a request
        string request = get_request(client);
        // break if client is done or an error occurred
        if (request.empty())
            break;


        string response = parse_request(request, client);

        // send responsed
        bool success = send_response(client,response);
        // break if an error occurred
        if (not success)
            break;
    }
    close(client);
}

string
Server::get_request(int client) {
    // read until we get a newline
    while (cache_.find("\n") == string::npos) {//check size
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        cache_.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    //change request to cache

    int nlpos = cache_.find("\n");

    string request = cache_.substr(0, nlpos + 1);
    logger_.log("---REQUESTTT: " + request);
    //trim cache_
    cache_.erase(0, nlpos + 1);

    return request;
    //return substring of cache
}

string
Server::get_request_message(int client, int length) {
    // read until we get a newline
    logger_.log("current cache: " + cache_);

    while (cache_.size() < length) {//check size
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        cache_.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    //change request to cache

    int nlpos = cache_.find("\n");

    string request = cache_.substr(0, length + 1);
    logger_.log("---REQUESTTT: " + request);
    //trim cache_
    cache_.erase(0, length + 1);

    return request;
    //return substring of cache
}

bool
Server::send_response(int client, string response) {
    logger_.log("sending response: ");
    logger_.log(response);
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}


