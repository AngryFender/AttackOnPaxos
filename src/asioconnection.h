#ifndef ASIOCONNECTION_H
#define ASIOCONNECTION_H

#include <iostream>
#include <string>
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void SetupAsioClient(const char* ipAddress)
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        tcp::resolver::query query(ipAddress,"daytime");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socker socket(io_context);
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
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcpv4()), 4000);

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

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service)
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
    boost::io_context _io_context;
    tcp::acceptor _acceptor;
public:
    TcpServer(boost::io_context &io_context) : _io_context(io_context), _acceptor(io_context, tcp::endpoint(tcp::v4(),4000))
    {
       start_accept();
    }
private:
    void start_accept()
    {
        TcpConnection::pointer new_connection= TcpConnection::create(io_context);
        _acceptor.async_accept(new_connection->socket(), [this, new_connection](boost::asio::placeholders::error)
        {
            this->handle_accept(new_connection);
        });
    }

    void handle_accept(TcpConnection::pointer new_connection)
    {
        if(!error)
        {
            new_connection->start();
        }

        start_accept();
    }
};


void SetupAsioServerAsync()
{
    try
    {
        boost::io_context io_context;
        TcpServer server(io_context);
        io_context.run();
    }
    catch(std::exception& e)
    {
       std::cerr << e.what() << "\n" ;
    }
}


#endif //ASIOCONNECTION_H
