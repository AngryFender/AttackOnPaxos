#include <iostream>

#include "../Shared/asioconnection.h"
#include "../Shared/plainconnection.h"



int main()
{
    std::cout << "Attack on Paxos!" << std::endl;
    //SetupSimpleServer();
    SetupAsioServerAsync();
    return 0;
}
