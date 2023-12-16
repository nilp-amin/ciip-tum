#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}

// TODO implement get_type function
std::string_view Audio::get_type() const
{
  return "AUD";
}

size_t Audio::get_raw_size() const {
  return byte_depth * sample_rate * num_channels * static_cast<size_t>(duration);
}

unsigned Audio::get_duration() { return this->duration; }

// TODO implement content update function
void Audio::update(FileContent&& new_content, unsigned new_duration)
{
  content = new_content;
  duration = new_duration;
}
