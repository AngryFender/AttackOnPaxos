#include <iostream>

#include "../Shared/asioconnection.h"
#include "../Shared/plainconnection.h"
#include "../Shared/ilogger.h"
#include "../Shared/logger.h"

void init()
{

}


int main()
{
    std::cout << "Attack on Paxos!" << std::endl;
    init();
    Logger::getInstance();
    Logger::getInstance().registerCallback([](LogType& type, std::string& message){
        std::cout<<message;
    });

    //SetupSimpleServer();
    SetupAsioServerAsync();
    return 0;
}
