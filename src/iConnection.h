
#ifndef ICONNECTION_H
#define ICONNECTION_H

class IConnection
{
public:
    void createConnection(const std::string& ip, const string& port);
    void sendMessage(const std::string& message);
    void requestMessage(std::string& message);
};

#endif //ICONNECTION_H
