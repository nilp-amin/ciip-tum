#include "logger.h"

#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

Logger::Logger(const std::string &filename) : file{} {
    // check if the file already exists
    if (fs::exists(filename))
    {
        // find the position of the last dot in the file name
        std::string backup_file_path{filename + "_old"};
        fs::copy(filename, backup_file_path, fs::copy_options::overwrite_existing);
    }

    file.open(filename);

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "Commencing logging for directory: "
         << fs::current_path() 
         << std::endl
         << "Current time is: "
         << std::ctime(&time)
         << std::endl;
}

Logger::~Logger()
{
    file.close();
}

void Logger::log(const std::string &path, status what) {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}
