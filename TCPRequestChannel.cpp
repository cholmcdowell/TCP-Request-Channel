#include "TCPRequestChannel.h"

using namespace std;


TCPRequestChannel::TCPRequestChannel (const std::string _ip_address, const std::string _port_no) {
    // if server
    //      create a socket on specified  port
    //          specify domain, type, and protocoal
    //      bind socket to address sets up listening
    //      mark socket as listening

    // if client
    //      create a socket on specified  port
    //          specify domain, type, and protocoal
    //      connect socket to IP addr of the server    

    if (_ip_address == ""){ // server
        // variables
        struct sockaddr_in server;

        // make socket
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        this ->sockfd = sock_fd;

        // machine info
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(stoi(_port_no));

        // bind
        if(bind(sock_fd, (struct sockaddr*) &server, sizeof(server)) < 0) {
            cout << "Binding Error\n"; 
        } 

        // listen
        listen(sock_fd, 256);

        // accept

    }
    else{ // client
        // variables
        struct sockaddr_in serv_info;
        int connfd; // connection fd

        // make socket
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        this ->sockfd = sock_fd;

        // machine info
        serv_info.sin_family = AF_INET;
        serv_info.sin_port = htons(stoi(_port_no));
        inet_pton(AF_INET, (char*)_ip_address.c_str(), &(serv_info.sin_addr));

        // connect
        if((connfd = connect(sock_fd, (struct sockaddr*) &serv_info, sizeof(serv_info))) < 0) { 
            cout << "Connect Error\n"; 
        } 
    }
}

TCPRequestChannel::TCPRequestChannel (int _sockfd) {
    this ->sockfd = _sockfd;
}

TCPRequestChannel::~TCPRequestChannel () {
    // close socket fd
    close(this ->sockfd);
}

int TCPRequestChannel::accept_conn () {
    // server will call this function
    // struct sockaddr_storage
    // wrapper for accept(...) - retval the sockfd of client
    
    // variables
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int accpt_fd = accept(this ->sockfd, (struct sockaddr*) &client, &client_len);

    return accpt_fd;
}

// read/write, recv/send
int TCPRequestChannel::cread (void* msgbuf, int msgsize) {
    ssize_t num_bytes; // bytes to read
    
    // read from socket
    num_bytes = read(this ->sockfd, msgbuf, msgsize);

    // return size read
    return num_bytes;

}

int TCPRequestChannel::cwrite (void* msgbuf, int msgsize) {
    ssize_t num_bytes; // bytes to write

    // write to socket
    num_bytes = write(this ->sockfd, msgbuf, msgsize);

    // return number of bytes written
    return num_bytes;

}
