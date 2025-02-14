#ifndef ASIOCONNECTION_H
#define ASIOCONNECTION_H

#include <iostream>

void SetupAsioClient(const char* ipAddress)
{
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);

        tcp::resolver::query query(ipAddress,"daytime");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socker socket(io_service);
        boost::system::error_code error= boost::asio::error::host_not_found;
        while(error && endpoint_iterator != end)
        {
            socket.close();
            socker.connect(*endpoint_iterator++, error);
        }

        if(error)
        {
            throw boost::system::system_error(error);
        }

        while(1)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof)
            {
                break; //Connection closed cleanly by peer
            }
            else
            {
                throw boost::system::system(error);
            }
            std::cout.write(buf.data(),len);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() <<"\n";
    }
}


#endif //ASIOCONNECTION_H
