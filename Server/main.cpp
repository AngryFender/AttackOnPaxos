#include <iostream>
#include <string>
#include "../Shared/asioconnection.h"
#include "../Shared/ilogger.h"
#include "../Shared/logger.h"

void init_logs()
{
    Logger::getInstance().registerCallback(LogType::DEBUG, [](const std::string& message)
    {
        std::cout<<message;
    });

    Logger::getInstance().registerCallback(LogType::INFO, [](const std::string& message)
    {
        std::cout<<message;
    });

    Logger::getInstance().registerCallback(LogType::ERROR, [](const std::string& message)
    {
        std::cerr<<message;
    });
    Log(INFO) << "Attack on Paxos!" << "\n";
}

void init_tcp_server()
{
    try
    {
        Log(INFO) << "Starting Server" << "\n";
        boost::asio::io_context io_context;
        TcpServer server(io_context);
        io_context.run();
    }
    catch (std::exception& e)
    {
        Log(ERROR) << e.what() << "\n";
    }
}

int main()
{
    init_logs();
    init_tcp_server();

    return 0;
}
