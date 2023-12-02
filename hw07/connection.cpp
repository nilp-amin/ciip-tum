#include "connection.h"

namespace net {

[[nodiscard]]
ssize_t send(int fd, std::span<const char> data)
{
    return ::send(fd, reinterpret_cast<const void*>(data.data()), data.size_bytes(), 0);
}

[[nodiscard]]
ssize_t receive(int fd, std::span<char> buf)
{
    return recv(fd, reinterpret_cast<void*>(buf.data()), buf.size_bytes(), 0);
}

Connection::Connection(FileDescriptor&& fd) : fd_{std::move(fd)} {}

void Connection::send(std::string_view data) const
{
    net::send(fd_.unwrap(), data);
}

void Connection::send(std::istream& data) const
{
    std::ostringstream msg;
    msg << data.rdbuf();
    send(msg.str());
}

[[nodiscard]]
ssize_t Connection::receive(std::ostream& stream) const
{
    const size_t buffer_size{128};
    char buffer[buffer_size];

    ssize_t bytes_read = recv(fd_.unwrap(), reinterpret_cast<void*>(buffer), buffer_size, 0);

    stream.write(buffer, bytes_read);

    return bytes_read;
}

[[maybe_unused]]
ssize_t Connection::receive_all(std::ostream& stream) const
{
    ssize_t total_bytes_read{0};
    ssize_t bytes_read{0};

    while ((bytes_read = receive(stream)) > 0)
    {
        total_bytes_read += bytes_read;
    }

    return total_bytes_read;
}

int Connection::fd() const
{
    return fd_.unwrap();
}

} // namespace net