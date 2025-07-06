#include <iostream>
#include "../Shared/connectionmanager.h"
#include "../Shared/acceptoradapter.h"
#include "../Shared/paxos.h"

#define LOCAL_PORTNO 3491

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
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait([&pax, &timer](const boost::system::error_code&)
    {
        restart_timer(timer, pax);
    });
}

void init_tcp_server()
{
    try
    {
        Log(INFO) << "Starting Server...listening on " << std::to_string(LOCAL_PORTNO).c_str() << "\n";
        boost::asio::io_context io_context;
        ConnectionManager connectionManager(io_context, LOCAL_PORTNO, std::make_shared<AcceptorAdapter>(io_context, LOCAL_PORTNO));

        Paxos pax(connectionManager, 1);
        std::thread external_thread([&pax, &io_context]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            io_context.post([&pax]()
            {
                pax.ContributeValue(69, [](const contribution_status& status)
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

        // boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(5));
        // timer.async_wait([&pax, &timer](const boost::system::error_code&)
        // {
        //     restart_timer(timer,pax);
        // });
        //
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
