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
    SetupAsioServerAsync();
}


int main()
{
    std::cout << "Attack on Paxos!" << "\n";
    init();

    return 0;
}
