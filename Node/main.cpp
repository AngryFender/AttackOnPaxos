#include <iostream>
#include "../Shared/connectionmanager.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/paxos.h"

#define PORTNO 3491

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

void restart_timer(boost::asio::deadline_timer& timer, Paxos& pax)
{
    // pax.ContributeValue(69);
    timer.expires_from_now(boost::posix_time::seconds(2));
    timer.async_wait([&pax, &timer](const boost::system::error_code&)
    {
        restart_timer(timer, pax);
    });
}

void init_tcp_server()
{
    try
    {
        Log(INFO) << "Starting Node" << "\n";
        boost::asio::io_context io_context;
        ConnectionManager connectionManager(io_context, PORTNO, std::make_shared<AcceptorAdapter>(io_context, PORTNO));

        const std::string address = "127.0.0.1";
        boost::asio::ip::basic_endpoint<tcp> end_point_node2(boost::asio::ip::address::from_string(address), 3490);
        std::shared_ptr<ISocketAdapter> socket_node2 = std::make_shared<SocketAdapter>(io_context);
        connectionManager.AddConnection(address, end_point_node2, socket_node2);

        boost::asio::ip::basic_endpoint<tcp> end_point_node3(boost::asio::ip::address::from_string(address), 3493);
        std::shared_ptr<ISocketAdapter> socket_node3 = std::make_shared<SocketAdapter>(io_context);
        connectionManager.AddConnection(address, end_point_node3, socket_node3);

        Paxos pax(connectionManager, 2);

        std::thread external_thread([&pax, &io_context]()
        {
            io_context.post([&pax]()
            {
                std::this_thread::sleep_for(std::chrono::seconds(10));
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

        // boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(2));
        // timer.async_wait([&pax, &timer](const boost::system::error_code&)
        // {
        //     restart_timer(timer,pax);
        // });

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
