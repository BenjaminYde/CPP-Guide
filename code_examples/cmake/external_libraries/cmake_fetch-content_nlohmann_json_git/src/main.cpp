#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <format>

int main() 
{
    // create the json data
    const std::string jsonData = R"({
        "key": "value1",
        "array": [
            {"arraykey": 1},
            {"arraykey": 2}
        ]
    })";

    // parse the json data
    nlohmann::json root;
    try
    {
        root = nlohmann::json::parse(jsonData);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << std::format("Error parsing JSON: {}", e.what()) << std::endl;
        return 1;
    }

    // access key
    if (root.contains("key"))
    {
        std::cout << std::format("{}", root["key"].get<std::string>()) << std::endl;
    }

    // access array
    for (const auto& item : root["array"]) 
    {
        std::cout << std::format("{}", item["arraykey"].get<int>()) << std::endl;
    }

    return 0;
}