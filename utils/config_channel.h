#pragma once

#include <string>
#include <unordered_map>

namespace adaptive_ai {

class ConfigChannel {
public:
    bool load(const std::string& filepath);
    std::string get(const std::string& key, const std::string& default_value = "") const;
    int get_int(const std::string& key, int default_value = 0) const;

private:
    std::unordered_map<std::string, std::string> config_map;
    void parse_line(const std::string& line);
};

} // namespace adaptive_ai
