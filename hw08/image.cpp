#include "image.h"

Image::Image(FileContent &&content, resolution_t res)
    : File{std::move(content)}, resolution{res} {}

// TODO implement get_type function
std::string_view Image::get_type() const
{
  return "IMG";
}

size_t Image::get_raw_size() const {
  return resolution[0] * resolution[1] * depth * per_pixel_size_t;
}

auto Image::get_resolution() const -> resolution_t { return this->resolution; }

// TODO implement content update function
void Image::update(FileContent&& new_content, resolution_t size)
{
  content = new_content;
  resolution = size;
}
