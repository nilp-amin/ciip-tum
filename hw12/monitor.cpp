#include "monitor.h"

#include <thread>

namespace fs = std::filesystem;

FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval},
    target_path_{targetpath}
{
    // TODO: Initialize a mapping for all files in the target path
    for (auto const& dir_entry : fs::directory_iterator{target_path_})
    {
        mapping_[dir_entry.path()] = fs::last_write_time(dir_entry);
    }
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    // TODO: Monitor the system for the given amount of time.
    // Every given interval, check for modifications to the files and log it

    // get the current time
    auto start_time = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - start_time < timeout)
    {
        std::cout << "in loop" << std::endl;
        // check for additions of files in the directory
        for (auto const& dir_entry : fs::directory_iterator(target_path_))
        {
            auto find_key_it = mapping_.find(dir_entry);

            if (find_key_it == mapping_.end())
            {
                // new file created
                mapping_[dir_entry.path()] = fs::last_write_time(dir_entry);
                logger.log(dir_entry.path(), status::added);
            }
        }

        // check for modifications or deletions of files in the directory
        for (auto const& file_info : mapping_)
        {
            if (not fs::exists(file_info.first))
            {
                // file deleted
                logger.log(file_info.first, status::removed);
                mapping_.erase(file_info.first);
            } else
            {
                auto new_write_time = fs::last_write_time(file_info.first);
                if (new_write_time != file_info.second)
                {
                    // file has been modified
                    mapping_[file_info.first] = new_write_time;
                    logger.log(file_info.first, status::changed);
                }
            }
        }

        std::this_thread::sleep_for(interval);
    }
}
