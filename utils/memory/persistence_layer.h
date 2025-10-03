#pragma once 
#include <iostream> 
#include <vector> 
#include <string> 

amespace adaptive_ai {
namespace memory {

enum class PersistStatus {
    Ok,
    IOError,
    NotFound,
    InvalidData
};

class PersistenceLayer {
public:
    explicit PersistenceLayer(std::string base_dir);

    // write data atomically (to base_dir/filename)
    PersistStatus write(const std::string& filename, const std::string& data);
    PersistStatus write(const std::string& filename, const std::vector<uint8_t>& data);

    // read data
    std::optional<std::string> read_string(const std::string& filename) const;
    std::optional<std::vector<uint8_t>> read_binary(const std::string& filename) const;

    // delete file
    PersistStatus remove(const std::string& filename);

    // check if file exists
    bool exists(const std::string& filename) const;

    // helper: get full path for a given file
    std::string path_for(const std::string& filename) const;

private:
    std::string m_base_dir;
    mutable std::mutex m_mutex;
    bool ensure_dir_exists() const;
};

} // namespace memory
} // namespace adaptive_ai

