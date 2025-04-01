#ifndef ASIOCONNECTION_H
#define ASIOCONNECTION_H

#include <ctime>
#include <iostream>
#include <string>
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "logger.h"

using boost::asio::ip::tcp;

void SetupAsioClient(const char* ipAddress)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        tcp::resolver::query query(ipAddress,"3490",boost::asio::ip::resolver_base::numeric_service);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socket socket(io_context);
        boost::system::error_code error= boost::asio::error::host_not_found;
        while(error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }

        if(error)
        {
            throw boost::system::system_error(error);
        }

        while(true)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof)
            {
                break; //Connection closed cleanly by peer
            }
            else if(error)
            {
                throw boost::system::system_error(error);
            }
            std::cout.write(buf.data(),len);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() <<"\n";
    }
}

std::string makeDateTimeString()
{
    const time_t now = std::time(nullptr);
    return ctime(&now);
}

void SetupAsioServer()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 4000));

        while (1)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string message = makeDateTimeString();
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
}
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
  typedef std::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer( new TcpConnection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        std::bind(&TcpConnection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  TcpConnection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class TcpServer
{
    boost::asio::io_context& _io_context;
    tcp::acceptor _acceptor;
public:
    explicit TcpServer(boost::asio::io_context& io_context) :
        _io_context(io_context),
        _acceptor(io_context, tcp::endpoint(tcp::v4(), 3490))
    {
       start_accept();
    }
private:
    void start_accept()
    {
        TcpConnection::pointer new_connection= TcpConnection::create(_io_context);
        _acceptor.async_accept(new_connection->socket(), std::bind(&TcpServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void handle_accept(TcpConnection::pointer new_connection, const boost::system::error_code& error)
    {
        if(!error)
        {
            new_connection->start();
        }

        start_accept();
    }
};

#endif //ASIOCONNECTION_H
