#ifndef PLAINCONNECTION_H
#define PLAINCONNECTION_H

#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void TryConnection()
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(( status = getaddrinfo(NULL,"3490",&hints, &servinfo)) != 0){
        std::cout<<"getaddrinfo error:"<< gai_strerror(status)<<"\n";
        return;
    }

    for(p = servinfo; p != NULL; p=p->ai_next)
    {
        void *addr;
        char *ipver;

        if(p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6*>(p->ai_addr);
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout<<ipver<<": "<<ipstr<<"\n" ;
    }
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);


    if(status = bind(sockfd, servinfo->ai_addr,INET6_ADDRSTRLEN) != 0)
    {
        std::cout << "bind error:" << gai_strerror(status) << "\n";
        return;
    }

    freeaddrinfo(servinfo);
}

#endif //PLAINCONNECTION_H
