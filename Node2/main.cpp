#include <iostream>
#include "../Shared/connectionmanager.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/heartbeat.h"
#include "../Shared/paxos.h"

#define LOCAL_PORTNO 3492
#define HEARTBEAT_TIMEOUT 1000
#define ACK_TIMEOUT 1000

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
        std::this_thread::sleep_for(std::chrono::seconds(2));
        Log(INFO) << "Starting Server...listening on " << std::to_string(LOCAL_PORTNO).c_str() << "\n";
        boost::asio::io_context io_context;
        Paxos pax(2);
        ConnectionManager connectionManager(&pax, std::make_shared<AcceptorAdapter>(io_context, LOCAL_PORTNO, HEARTBEAT_TIMEOUT, ACK_TIMEOUT));

        const std::string address = "127.0.0.1";
        boost::asio::ip::basic_endpoint<tcp> end_point(boost::asio::ip::address::from_string(address), 3491);

        connectionManager.AddConnection(end_point, std::make_shared<Connection>(io_context, std::make_shared<Heartbeat>(io_context, HEARTBEAT_TIMEOUT, ACK_TIMEOUT)));

        std::thread external_thread([&pax, &io_context]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(20));
            io_context.post([&pax]()
            {
                pax.ContributeValue(71, [](const contribution_status& status)
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

int main(int argc, char* argv[])
{
    init_logs();
    init_tcp_server();
    return 0;
}
