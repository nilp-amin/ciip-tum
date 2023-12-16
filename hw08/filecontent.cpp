#include "filecontent.h"

// TODO implement constructors
FileContent::FileContent(const std::string& content) : data_{content} {}
FileContent::FileContent(std::string&& content) : data_{content} {}
FileContent::FileContent(const char* content) : data_{content} {}

FileContent::FileContent(const FileContent& other) : data_{other.data_} {}
FileContent& FileContent::operator=(const FileContent& other)
{
    data_ = other.data_; 
    return *this;
}
FileContent::FileContent(FileContent&& other) noexcept : data_{std::move(other.data_)} {}
FileContent& FileContent::operator=(FileContent&& other) noexcept
{
    if (this != &other)
    {
        data_ = std::move(other.data_);
        other.data_ = "";
    }

    return *this;
}


// TODO implement member functions
size_t FileContent::get_size() const
{
    return data_.size();
}

std::shared_ptr<const std::string> FileContent::get() const
{
    if (data_.empty())
    {
        return nullptr;
    }
    return std::make_shared<const std::string>(data_);
}