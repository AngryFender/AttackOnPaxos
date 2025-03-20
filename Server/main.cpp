#include <iostream>
#include <string>
#include "../Shared/asioconnection.h"
#include "../Shared/ilogger.h"
#include "../Shared/logger.h"

void init()
{
    Logger::getInstance().registerCallback(LogType::DEBUG, [](const std::string& message)
    {
        std::cout<<message<<"\n";
    });

    Logger::getInstance().registerCallback(LogType::INFO, [](const std::string& message)
    {
        std::cout<<message<<"\n";
    });

    Logger::getInstance().registerCallback(LogType::ERROR, [](const std::string& message)
    {
        std::cerr<<message<<"\n";
    });

    SetupAsioServerAsync();
}

int main()
{
    init();
    Log(INFO)<< "Attack on Paxos!" << "\n";

    return 0;
}
