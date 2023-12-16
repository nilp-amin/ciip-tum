#include "document.h"


Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO implement get_type function
std::string_view Document::get_type() const
{
  return "DOC";
}

size_t Document::get_raw_size() const {
  // TODO get the document size
  return this->get_size();
}

unsigned Document::get_character_count() const {
  // TODO count non whitespace characters
  std::string data{*content.get()};
  return std::count_if(data.begin(), data.end(), 
    [](char c) {
      return !std::isspace(static_cast<unsigned char>(c));
    }
  );
}

// TODO implement content update function
void Document::update(FileContent&& new_content)
{
  content = new_content;
}
