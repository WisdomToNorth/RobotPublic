#include "rfjson.h"

#include <fstream>
#include <iostream>

#include "datautils.h"
#include "nlohmann/json.hpp"

namespace rfbase
{
bool JsonUtils::loadJson(const std::string &path, nlohmann::json &json)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Could not open file for reading: " << path << '\n';
        return false;
    }

    try
    {
        json = nlohmann::json::parse(file);
        file.close();
    }
    catch (nlohmann::json::parse_error &e)
    {
        std::cerr << "JSON ERROR: " << e.what() << '\n';
        file.close();
        return false;
    }

    return true;
}

bool JsonUtils::saveJson(const nlohmann::json &obj, const std::string &path)
{
    std::ofstream file(path, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open())
    {
        std::cout << "File open error";
        return false;
    }

    try
    {
        // Serialize the object to a string and write it to the file. 4 is for
        // pretty printing.
        file << obj.dump(4);
        file.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught while saving JSON: " << e.what() << '\n';
        file.close();
        return false;
    }

    return true;
}
} // namespace rfbase
