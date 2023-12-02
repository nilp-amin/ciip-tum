#include "socket.h"

namespace net {

bool is_listening(int fd)
{
    int is_listening;
    socklen_t len{sizeof(is_listening)};
    getsockopt(
        fd, 
        SOL_SOCKET, 
        SO_ACCEPTCONN, 
        &is_listening, 
        &len
    );

    return is_listening;
}

Socket::Socket() : fd_{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)} {}

void Socket::listen(uint16_t port) const
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(fd_.unwrap(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    ::listen(fd_.unwrap(), 5);
}

Connection Socket::accept() const
{
    // make sure the fd is already listening
    if (!is_listening(fd_.unwrap()))
    {
        throw std::runtime_error{"Socket is not listening."};
    }
    FileDescriptor fd{::accept(fd_.unwrap(), NULL, NULL)};

    return Connection{std::move(fd)};
}

Connection Socket::connect(std::string destination, uint16_t port)
{
    if (destination == "localhost")
    {
        destination = "127.0.0.1";
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(destination.c_str());

    ::connect(fd_.unwrap(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    return Connection{std::move(fd_)};
}

Connection Socket::connect(uint16_t port)
{
    return connect("127.0.0.1", port);
}

int Socket::fd() const
{
    return fd_.unwrap();
}

} // namespace net