#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               std::shared_ptr<File> file) {
  // TODO: Do sanity checks first! Is that actually a good name, is the pointer set,
  //       does a file with that name already exists? Anything else to check here?
  auto already_exists = files_.find(name);
  if (file == nullptr || name.empty() || already_exists != files_.end())
  {
    return false;
  }

  // Creater a shared pointer to the this object
  auto thisptr = this->shared_from_this();

  // TODO: Check if the file already has a filesystem, if so, it may only be
  //       the same as this one, not another one!
  if (!file->file_system_.expired() && file->file_system_.lock().get() != thisptr.get())
  {
    return false;
  }

  file->file_system_ = std::move(thisptr);

  // TODO: Now actually store it in the filesystem
  files_[name] = file;

  return true;
}

bool Filesystem::remove_file(std::string_view name) {
  // TODO: Remove file from the filesystem, return true if successful, false otherwhise.
  //       false may indicate multple different reasons for failure, which is not impotant

  auto file = files_.find(std::string{name});
  if (file == files_.end())
  {
    return false;
  }

  files_.erase(file);

  return true;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {
  // TODO: Check filesystem for the source and dest files, if it's possible to rename
  //       the source file, then update the filesystem
  auto file = files_.find(std::string{source});
  auto already_exists = files_.find(std::string{dest});
  if (file == files_.end() || already_exists != files_.end() || source.empty() || dest.empty())
  {
    return false; 
  }

  register_file(std::string{dest}, file->second);
  return remove_file(source);
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {
  // TODO: Check the filesystem for the given file, return it if you found it,
  //       else just return nothing
  auto file = files_.find(std::string{name});
  if (file->first == std::string(name))
  {
    return file->second;
  }
  return nullptr;
}

size_t Filesystem::get_file_count() const {
  // TODO: how many files have you got?
  return files_.size();
}

size_t Filesystem::in_use() const {
  // TODO: sum up all real file sizes
  size_t sum{0};
  std::for_each(files_.begin(), files_.end(), [&sum](const auto& pair)
    {
      sum += pair.second->get_size();
    }
  );
  return sum;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  for (auto&& entry : this->files_) {
      // TODO: fix printing name, type and size
      output << entry.first << std::endl;
  }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const {
  // TODO: find the matching files and return them
  std::vector<std::shared_ptr<File>> result;
  for (const auto& pair : files_)
  {
    auto raw_size = pair.second->get_raw_size();
    if (raw_size <= max && raw_size >= min)
    {
      result.push_back(pair.second);
    }
  }
  return result;
}
