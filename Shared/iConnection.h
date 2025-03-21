#ifndef ICONNECTION_H
#define ICONNECTION_H
#include <functional>
#include <string>

class IConnection
{
public:
    virtual ~IConnection() = default;
    virtual void createConnection(const std::string& ip, const std::string& port) = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void registerMessageReceived(std::function<void(const std::string& message)> handler) = 0;
};

#endif //ICONNECTION_H
