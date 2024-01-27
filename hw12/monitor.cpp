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
    for (auto const& dir_entry : fs::recursive_directory_iterator{target_path_})
    {
        mapping_[dir_entry.path()] = fs::last_write_time(dir_entry);
    }
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    // get the current time
    auto start_time = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - start_time < timeout)
    {
        // check for additions of files in the directory
        for (auto const& dir_entry : fs::recursive_directory_iterator(target_path_))
        {
            auto find_key_it = mapping_.find(dir_entry.path());

            if (find_key_it == mapping_.end())
            {
                // new file created
                mapping_[dir_entry.path()] = fs::last_write_time(dir_entry);
                logger.log(dir_entry.path(), status::added);
            }
        }

        // check for modifications or deletions of files in the directory
        for (auto it = mapping_.begin(); it != mapping_.end();)
        {
            if (not fs::exists(it->first))
            {
                // file deleted
                logger.log(it->first, status::removed);
                it = mapping_.erase(it);
            } else
            {
                auto new_write_time = fs::last_write_time(it->first);
                if (new_write_time != it->second)
                {
                    // file has been modified
                    mapping_[it->first] = new_write_time;
                    logger.log(it->first, status::changed);
                }
                ++it;
            }
        }

        std::this_thread::sleep_for(interval);
    }
}
