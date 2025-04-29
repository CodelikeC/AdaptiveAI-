#include "utils/config_channel.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace adaptive_ai {

bool ConfigChannel::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        parse_line(line);
    }
    return true;
}

void ConfigChannel::parse_line(const std::string& line) {
    std::string trimmed = line;
    trimmed.erase(remove_if(trimmed.begin(), trimmed.end(), ::isspace), trimmed.end());

    if (trimmed.empty() || trimmed[0] == '#') return;

    size_t pos = trimmed.find('=');
    if (pos != std::string::npos) {
        std::string key = trimmed.substr(0, pos);
        std::string value = trimmed.substr(pos + 1);
        config_map[key] = value;
    }
}

std::string ConfigChannel::get(const std::string& key, const std::string& default_value) const {
    auto it = config_map.find(key);
    return (it != config_map.end()) ? it->second : default_value;
}

int ConfigChannel::get_int(const std::string& key, int default_value) const {
    auto value = get(key);
    if (!value.empty()) {
        try {
            return std::stoi(value);
        } catch (...) {}
    }
    return default_value;
}

} // namespace adaptive_ai
