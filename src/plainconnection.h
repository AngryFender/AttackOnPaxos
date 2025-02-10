#ifndef PLAINCONNECTION_H
#define PLAINCONNECTION_H

#include <cstring>
#include <iostream>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void SetupSimpleServer()
{
    struct utsname unameData;
    uname(&unameData);
    std::cout<<"SERVER ⬢ " << unameData.nodename <<" \n";

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


    if(status = listen(sockfd, 10) != 0)
    {
        std::cout << "listen error:" << gai_strerror(status) <<"\n";
        return;
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;
    int new_socketfd;

    if((new_socketfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_size)) == -1)
    {
        int err = errno;
        std::cout <<"accept error: " << strerror(err)<<"\n";
        return;
    }

    char* msg = "Hello Paxos!";

    const int len = strlen(msg);
    int bytes_sent = send(new_socketfd, msg, len, 0);


    std::cout<<"total byes sent"<< bytes_sent <<"\n";


    freeaddrinfo(servinfo);
}


void SetupSimpleClient()
{
    struct utsname unameData;
    uname(&unameData);
    std::cout<<"CLIENT ⬢ " << unameData.nodename <<" \n";

    addrinfo hint;
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;

    addrinfo *servinfo;
    int status;
    if((status = getaddrinfo("angryfedora", "3490",&hint, &servinfo)) !=0){
        std::cout<<"getaddrinfo error:"<< gai_strerror(status)<<"\n";
        return;
    }

    int socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    std::cout<<"Attempting to connect\n";
    connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen);


}

#endif //PLAINCONNECTION_H
