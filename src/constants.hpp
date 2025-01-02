#pragma once

#include "../ollama/ollama.hpp" // For JSON
#include <SFML/Graphics/Color.hpp>

using json = nlohmann::json;

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<sf::Color> {
    static void to_json(json &j, const sf::Color &color) {
        j = json{
            {"r", color.r}, {"g", color.g}, {"b", color.b}, {"a", color.a}};
    }

    static void from_json(const json &j, sf::Color &color) {
        color.r = j.at("r").get<std::uint8_t>();
        color.g = j.at("g").get<std::uint8_t>();
        color.b = j.at("b").get<std::uint8_t>();
        color.a = j.at("a").get<std::uint8_t>();
    }
};

// template <> struct adl_serializer<sf::Uint32> {
//     static void to_json(json &j, const sf::Uint32 &value) { j = value; }
//
//     static void from_json(const json &j, sf::Uint32 &value) {
//         value = j.get<sf::Uint32>();
//     }
// };
NLOHMANN_JSON_NAMESPACE_END

// Class for easy access to Constants
// Constants are stored in a JSON file
// Opened at the start of the program
// no need to recompile to change constants
class Constants {
    public:
    Constants(std::string path) {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Cannot open file: " << path << std::endl;
            exit(1);
        }
        file >> this->j;

        // print the JSON object
        std::cout << this->j.dump(4) << std::endl;
    }

    template <typename T = std::string> T get(std::string key) const {
        // Check if key exists
        if (!this->j.contains(key)) {
            std::cerr << "Key not found: " << key << std::endl;
            exit(1);
        }
        return this->j[key].template get<T>();
    }

    std::string operator[](std::string key) const {
        return this->get<std::string>(key);
    }

    private:
    json j;
};

// Global Constants object
// C++ 17 inline variables
inline const Constants cst("config.json");
