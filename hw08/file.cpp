#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
  // TODO: Check that a filesystem actually exists, then rename it in the filesystem
  if (!name.empty())
  {
    name = new_name;
    return true;
  }
  return false;
}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

// TODO file constructor
File::File(FileContent&& content, std::string_view name) 
: content{content}, name{name} {}
