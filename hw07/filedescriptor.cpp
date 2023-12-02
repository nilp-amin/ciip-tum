#include "filedescriptor.h"

// TODO Implement the methods of the FileDescriptor class as given in the filedescriptor.h header.

namespace net {

FileDescriptor::FileDescriptor() {};

FileDescriptor::FileDescriptor(int fd) : fd_{fd} {}

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : 
    fd_{std::move(other.fd_)}
{
    other.fd_ = -1;
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept
{
    // check for self assignment
    if (this == &other)
    {
        return *this;
    }

    fd_ = std::move(other.fd_);
    other.fd_ = -1;

    return *this;

}

FileDescriptor::~FileDescriptor()
{
    if (fd_.has_value())
    {
        close(fd_.value());
    }
}

int FileDescriptor::unwrap() const
{
    if (fd_.has_value())
    {
        return fd_.value();
    }

    return -1;
}

} // namespace net
