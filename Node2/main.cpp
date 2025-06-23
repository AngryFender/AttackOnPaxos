#include <iostream>
#include "../Shared/connectionmanager.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/paxos.h"

#define PORTNO 3493

void init_logs()
{
    Logger::getInstance().registerCallback(LogType::DEBUG, [](const std::string& message)
    {
        std::cout << message;
    });

    Logger::getInstance().registerCallback(LogType::INFO, [](const std::string& message)
    {
        std::cout << message;
    });

    Logger::getInstance().registerCallback(LogType::ERROR, [](const std::string& message)
    {
        std::cerr << message;
    });
    Log(INFO) << "Attack on Paxos!" << "\n";
}

void init_tcp_server()
{
    try
    {
        Log(INFO) << "Starting Node" << "\n";
        boost::asio::io_context io_context;
        ConnectionManager connectionManager(io_context, PORTNO, std::make_shared<AcceptorAdapter>(io_context, PORTNO));

        const std::string address = "127.0.0.1";
        boost::asio::ip::basic_endpoint<tcp> end_point(boost::asio::ip::address::from_string(address), 3490);
        std::shared_ptr<ISocketAdapter> socket = std::make_shared<SocketAdapter>(io_context);

        connectionManager.AddConnection(address, end_point, socket);
        Paxos pax(connectionManager,2);

        io_context.run();
    }
    catch (std::exception& e)
    {
        Log(ERROR) << e.what() << "\n";
    }
}

int main(int argc, char* argv[])
{
    init_logs();
    init_tcp_server();
    return 0;
}
