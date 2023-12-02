#pragma once

#include <optional>
#include <utility>

#include <unistd.h>

namespace net {

/// Wrapper for a Linux style file descriptor. It represents a potentially empty file descriptor.
/// The wrapper represents a unique ownership model, i.e. the file descriptor will get invalided
/// with the end of the lifetime of the object.
///
/// The following code should help you understand the ownership model:
/// ```cpp
/// auto fdraw {get_socket_fd_from_somewhere()};
/// {
///     FileDescriptor fd{fdraw};
///
///     // use fd, do whatever with it
/// }
/// // fd should not be usable anymore
/// ```
class FileDescriptor {
public:
    /// Default constructor for empty file descriptor
    FileDescriptor();

    /// Construct from a integer file descriptor returned from the C API, take ownership of the
    /// descriptor
    explicit FileDescriptor(int fd);

    /// Close the file descriptor (if present and valid)
    /// Check out: close(3)
    ~FileDescriptor();

    // TODO: Implement both copy and move constructors and assignment operators for the ownership model
    //       described in the class description.

    // delete the copy constructor
    FileDescriptor(const FileDescriptor&) = delete;
    // delete the copy assignment operator
    FileDescriptor& operator=(const FileDescriptor& other) = delete;

    // move constructor
    FileDescriptor(FileDescriptor&& other) noexcept;
    // move assignment operator
    FileDescriptor& operator=(FileDescriptor&& other) noexcept;

    /// Return the underlying file descriptor, if not present return -1 (this is quite standard for
    /// linux systems)
    int unwrap() const;

private:
    std::optional<int> fd_ {};
};
} // namespace net
