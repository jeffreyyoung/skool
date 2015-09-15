#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
    logger_ = Logger();
}

Client::~Client() {
}

void Client::run(bool isActivated) {

    logger_.set_is_activated(isActivated);

    // connect to the server and run echo program
    create();
    echo();
}

void
Client::create() {
}

void
Client::close_socket() {
}

vector<string>
Client::split_by_line(string str){
    vector<string> lines;
    istringstream stream(str);
    string line;
    while(getline(stream, line)) {
        lines.push_back(line);
    }

    return lines;
}

vector<string>
Client::split(string str){
    string buf; // Have a buffer string
    stringstream ss(str); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf)
        tokens.push_back(buf);

    return tokens;
}

string
Client::get_user_message(){
    cout << "- Type your message. End with a blank line -" << endl;
    string message = "";
    string line;
    while (getline(cin, line)) {
        if (line == "")
            return message.substr(0, message.size()-1);

        message += line + "\n";
    }


}

string 
Client::handle_response(string response, string type){

    logger_.log("handling response: " + response);

    if (response == fail_){
        return fail_;
    }

    vector<string> lines = split_by_line(response);
    vector<string> tokens = split(lines.at(0));
    if (tokens.at(0) == "error"){
        string error = response.substr(tokens.at(0).size(), response.size());
        return "Server error: " + error;
    }
    if (type == "send"){
        if (response == fail_)
            return fail_;
        else if (response == "OK\n")
            return "";
    }
    else if (type == "list"){
        string word = tokens.at(0);
        string num = tokens.at(1);
        return response.substr(lines.at(0).size() + 1, response.size());
    }
    else if (type == "read"){
        string word = tokens.at(0);
        if (word == "message") {
            string subject = tokens.at(1);
            int size = stoi(tokens.at(2));
            //edit this
            string message = response.substr(lines.at(0).size(), response.size());
            return subject + message;
        }
    }
    else {
        return "";
    }
}

string
Client::handle_command(string line) {
    vector<string> fields = split(line);

    logger_.log("handling command: " + line);

    if(fields.size() == 0){
        return "";
    }
    else if (fields.at(0) == "quit"){
        return fail_;
    }
    else if (fields.at(0) == "send" && fields.size() == 3) {
        string name = fields.at(1);
        string subject = fields.at(2);
        string message = get_user_message();
        string length = to_string(message.size());
        string request = "put " + name + " " + subject + " " + length + "\n" + message;
        send_request(request);//break on fail?

        string response = get_response();

        return handle_response(response, "send");
        

    }
    else if (fields.at(0) == "list" && fields.size() == 2) {
        string name = fields.at(1);
        string request = "list " + name + "\n";
        send_request(request);

        string response = get_response();
        
        return handle_response(response, "list");
    }
    else if (fields.at(0) == "read" && fields.size() == 3) {
        string name = fields.at(1);
        string index = fields.at(2);
        string request = "get " + name + " " + index + "\n";
        send_request(request);

        string response = get_response();

        return handle_response(response,"read");
        
    }
    else {
        return "Command not recognized...\n";
    }
}

void
Client::echo() {
    string line;
    cout << "% ";
    // loop to handle user interface
    while (getline(cin,line)) {
        // append a newline
        //line += "\n";
        // send request
        
        string response = handle_command(line);
        if (response == fail_) break;
        cout << response;

        cout << "% ";
    }
    close_socket();
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
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

string
Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return fail_;
        } else if (nread == 0) {
            // the socket is closed
            return fail_;
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    //cout << "server response: " + response;
    return response;
}
