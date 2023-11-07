#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>

using namespace contact_list;

bool contact_list::add(storage& contacts, std::string_view name, number_t number)
{
    std::vector<std::string>::iterator name_iter = std::find(contacts.names.begin(), contacts.names.end(), std::string(name));
    if (name.empty() || name_iter != contacts.names.end())
    {
        // empty name provided or duplicate name provided
        return false;
    }

    contacts.numbers.push_back(number);
    contacts.names.push_back(std::string(name));
    return true;
}

size_t contact_list::size(const storage& contacts)
{
    return contacts.names.size();
}

number_t contact_list::get_number_by_name(storage& contacts, std::string_view name)
{

    std::string name_conver = std::string(name);
    for (size_t i = 0; i < contacts.names.size(); ++i)
    {
        // find index of matching name
        if (name_conver == contacts.names[i])
        {
            return contacts.numbers[i];
        }
    }

    // No contact of give name was found in the contacts list
    return -1;
}

std::string contact_list::to_string(const storage& contacts)
{
    std::stringstream output;
    for (size_t i = 0; i < contacts.names.size(); ++i)
    {
        output << contacts.names[i]
               << " - " << std::left << std::setw(10) << contacts.numbers[i] << std::endl;
    }

    return output.str();
}

bool contact_list::remove(storage& contacts, std::string_view name)
{
    std::vector<std::string>::iterator name_iter = std::find(contacts.names.begin(), contacts.names.end(), std::string(name));
    if (name_iter != contacts.names.end())
    {
        // calculate the index of the iterator
        size_t offset = name_iter - contacts.names.begin();
        contacts.names.erase(name_iter);
        contacts.numbers.erase(contacts.numbers.begin() + offset);

        return true;
    }

    // requested name was not part of the list
    return false;
}

void contact_list::sort(storage& contacts)
{
    std::vector<std::pair<std::string, number_t>> zipped_contacts;

    // zip contacts together
    for (size_t i = 0; i < contacts.names.size(); ++i)
    {
        zipped_contacts.push_back(std::make_pair(contacts.names[i], contacts.numbers[i]));
    }

    // sort by name - std::sort already has lexographic implemented
    std::sort(zipped_contacts.begin(), zipped_contacts.end());

    // unzup contacts and apply changes to original data structure
    for (size_t i = 0; i < contacts.names.size(); ++i)
    {
        contacts.names[i] = zipped_contacts[i].first;
        contacts.numbers[i] = zipped_contacts[i].second;
    }
}

std::string contact_list::get_name_by_number(storage& contacts, number_t number)
{
    for (size_t i = 0; i < contacts.numbers.size(); ++i)
    {
        // find index of matching number 
        if (number == contacts.numbers[i])
        {
            return contacts.names[i];
        }
    }

    // No contact of give name was found in the contacts list
    return "";
}