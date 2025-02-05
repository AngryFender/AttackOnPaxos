#ifndef PLAINCONNECTION_H
#define PLAINCONNECTION_H

#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


void TryConnection()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(( status = getaddrinfo(NULL,"8080",&hints, &servinfo)) != 0){
        std::cout<<"getaddrinfo error:"<< gai_strerror(status)<<"\n";
    }

    freeaddrinfo(servinfo);
}

#endif //PLAINCONNECTION_H
