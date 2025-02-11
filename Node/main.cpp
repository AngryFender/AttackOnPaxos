#include <iostream>
#include "../src/plainconnection.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting Node" << std::endl;

    if(argc < 2)
    {
        std::cout<< "ip-address missing as param\n";
        return -1;
    }

    SetupSimpleClient(argv[1]);

    return 0;
}
