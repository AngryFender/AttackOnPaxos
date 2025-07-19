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

#define LOCAL_PORTNO 3493

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
        std::this_thread::sleep_for(std::chrono::seconds(4));
        Log(INFO) << "Starting Server...listening on " << std::to_string(LOCAL_PORTNO).c_str() << "\n";
        boost::asio::io_context io_context;
        Paxos pax(3);
        ConnectionManager connectionManager(&pax, std::make_shared<AcceptorAdapter>(io_context, LOCAL_PORTNO));
        const std::string address = "127.0.0.1";
        boost::asio::ip::basic_endpoint<tcp> end_point_node1(boost::asio::ip::address::from_string(address), 3491);
        connectionManager.AddConnection(end_point_node1, std::make_shared<Connection>(io_context));

        boost::asio::ip::basic_endpoint<tcp> end_point_node2(boost::asio::ip::address::from_string(address), 3492);
        connectionManager.AddConnection(end_point_node2, std::make_shared<Connection>(io_context));

        std::thread external_thread([&pax, &io_context]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            io_context.post([&pax]()
            {
                pax.ContributeValue(70, [](const contribution_status& status)
                {
                    if (status == contribution_status::success)
                    {
                        Log(INFO) << "Contribution successful\n";
                    }
                    else
                    {
                        Log(INFO) << "Contribution failed\n";
                    }
               });
           });
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
