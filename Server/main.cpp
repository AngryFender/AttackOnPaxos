#include <iostream>

#include "src/asioconnection.h"
//#include "src/plainconnection.h"



int main()
{
    std::cout << "Attack on Paxos!" << std::endl;
    //SetupSimpleServer();
    SetupAsioServerAsync();
    return 0;
}
