#include "client.h"

namespace net {

Client::Client() : socket{} {} 

Connection Client::connect(std::string destination, uint16_t port)
{
    return socket.connect(destination, port);
}

Connection Client::connect(uint16_t port)
{
    return connect("localhost", port);
}

} // namespace net