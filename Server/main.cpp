#include <iostream>
#include <string>
#include "../Shared/ilogger.h"
#include "../Shared/logger.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/connectionmanager.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/istrategy.h"
#include "../Shared/paxos.h"
#include "../Shared/utility.h"

#define PORTNO 3490

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
        ConnectionManager connectionManager(io_context, PORTNO, std::make_shared<AcceptorAdapter>(io_context, PORTNO));
        Paxos pax(connectionManager,1);

        boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(2));
        timer.async_wait([&pax](const boost::system::error_code&)
        {
            pax.SendPrepare(21);
        });

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
