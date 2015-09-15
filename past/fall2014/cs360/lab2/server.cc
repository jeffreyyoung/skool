#include "server.h"

Server::Server() {
    // setup variables
    //buflen_ = 1024;
    //buf_ = new char[buflen_+1];
    num_threads = 10;
}

Server::~Server() {
    delete mailbox_sem;
    delete queue_sem;
    delete n;
    delete e;
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


    mailbox_sem = sem_open("mailbox", O_CREAT, 0644, 1); 
    queue_sem  = sem_open("queue", O_CREAT, 0644, 1);
    n = sem_open("n", O_CREAT, 0644, 0);
    e = sem_open("e", O_CREAT, 0644, num_threads);


    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++){
        pthread_create(&threads[i], NULL, Server::callMemberFunction, this);
    }

      // accept clients
    sem_wait(mailbox_sem);
    logger_.log("Accepting clients");
    sem_post(mailbox_sem);
    while (1) {

        if((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0){
            int semVal;
            sem_getvalue(e, &semVal);

            cout << "accepted a client!!!   " << semVal << endl;

            sem_wait(e);
            cout << "waiting 1" << endl;
            sem_wait(queue_sem);
            clients_queue.push(client);
            sem_post(queue_sem);
            sem_post(n);
        }

    }

    sem_close(mailbox_sem);
    sem_close(queue_sem);
    sem_close(n);
    sem_close(e);

    sem_unlink("mailbox");
    sem_unlink("queue");
    sem_unlink("n");
    sem_unlink("e");

    close_socket();
}

void* 
Server::runThread(){
    sem_wait(mailbox_sem);
    logger_.log("running thread");
    sem_post(mailbox_sem);
    while(true){
        int semVal;
        sem_getvalue(n, &semVal);
        cout << "*" << endl;
        sem_wait(n);
        cout << "$" << endl;
        sem_wait(queue_sem);
        int client = clients_queue.front();
        clients_queue.pop();
        sem_post(queue_sem);
        sem_post(e);

        handle(client);
    }

    return (void*)"";
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
    sem_wait(mailbox_sem);
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
    sem_post(mailbox_sem);
}

string
Server::parse_request(string request, int client, string &cache_){
    logger_.log("parsing request: " + request);
    vector<string> fields = split(request);

    for (int i = 0; i < fields.size(); i++){
        logger_.log(to_string(i) + " " + fields.at(i));
    }

    if (fields.size() == 0){
        return "error invalid message\n";
    }
    else if (fields.at(0) == "reset"){
        sem_wait(mailbox_sem);
        map_.clear();
        sem_post(mailbox_sem);
        //reset message
        return "OK\n";
    }
    else if (fields.at(0) == "put" && fields.size() >= 4) {
        string name = fields.at(1);
        string subject = fields.at(2);
        int length = atoi(fields.at(3).c_str());
        logger_.log("about to get data...");
        string data = get_request_message(client, length, cache_);
        //CHASDFJKALSDF JALKSDF ATHIS!!!!!
        //string data = get_request(client, cache_);
        addToMap(name, subject, data, length);
        return "OK\n";
    }
    else if (fields.at(0) == "list" && fields.size() ==2){
        string name = fields.at(1);
        sem_wait(mailbox_sem);

        map<string, DATA>::iterator it = map_.find(name);

        if (it != map_.end()){
            logger_.log("listing data");

            string str = "list " + to_string(map_[name].subjects.size()) + "\n";
            cout << str << endl;
            for (int i = 0; i < map_[name].subjects.size(); i++){
                str += to_string(i + 1) + " " + map_[name].subjects.at(i) + "\n";
            }
            sem_post(mailbox_sem);
            return str;
        }
        else {
            sem_post(mailbox_sem);
            return "error user not found\n";
        }

        return "list\n";
    }
    else if (fields.at(0) == "get" && fields.size() == 3){
        string name = fields.at(1);
        int index = atoi(fields.at(2).c_str());
        index--;
        sem_wait(mailbox_sem);
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
            sem_post(mailbox_sem);
            return response;
        }
        else {
            sem_post(mailbox_sem);
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

    string cache_ = "";
    while (1) {
        // get a request
        string request = get_request(client, cache_);
        // break if client is done or an error occurred
        if (request.empty())
            break;


        string response = parse_request(request, client, cache_);

        // send responsed
        bool success = send_response(client,response);
        // break if an error occurred
        if (not success)
            break;
    }
    close(client);
}

string
Server::get_request(int client, string &cache_) {
    // read until we get a newline
    int buflen_ = 1024;
    char *buf_ = new char[buflen_+1];
    logger_.log("current cache: " + cache_);
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
    delete buf_;
    return request;
    //return substring of cache
}

string
Server::get_request_message(int client, int length, string &cache_) {
    // read until we get a newline
    logger_.log("current cache: " + cache_);
    logger_.log("getting message by length: " + cache_);
    logger_.log("desired length: " + to_string(length) + ", cache length: " + to_string(cache_.size()));
    int buflen_ = 1024;
    char *buf_ = new char[buflen_+1];

    while (cache_.size() < length) {//check size
        logger_.log("in while loop");
        int nread = recv(client,buf_,1024,0);
        logger_.log("in while loop1");
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

    //int nlpos = cache_.find("\n");

    string request = cache_.substr(0, length + 1);
    logger_.log("---REQUESTTT: " + request);
    //trim cache_
    cache_.erase(0, length + 1);
    delete buf_;
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


