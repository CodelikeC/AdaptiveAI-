#include "persistence_layer.h"
using namespace std; 

#include <iostream>
#include <fstream> 
#include <filesystem> 
#include <stdexcept>

namespace adaptive_ai 
{
    namespace memory
    {
        namespace fs = std::filesystem;

PersistenceLayer::PersistenceLayer(std::string base_dir)
    : m_base_dir(std::move(base_dir)) {
    ensure_dir_exists();
}

bool PersistenceLayer::ensure_dir_exists() const {
    try {
        if (!fs::exists(m_base_dir)) {
            return fs::create_directories(m_base_dir);
        }
        return true;
    } catch (...) {
        return false;
    }
}

std::string PersistenceLayer::path_for(const std::string& filename) const {
    return (fs::path(m_base_dir) / filename).string();
}

PersistStatus PersistenceLayer::write(const std::string& filename, const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    return write(filename, bytes);
}

PersistStatus PersistenceLayer::write(const std::string& filename, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> lk(m_mutex);
    if (!ensure_dir_exists()) return PersistStatus::IOError;

    auto path = fs::path(m_base_dir) / filename;
    auto tmp_path = path;
    tmp_path += ".tmp";

    try {
        // write to temp file
        {
            std::ofstream ofs(tmp_path, std::ios::binary | std::ios::trunc);
            if (!ofs.is_open()) return PersistStatus::IOError;
            ofs.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
            ofs.flush();
            if (!ofs.good()) return PersistStatus::IOError;
        }
        // atomic rename
        fs::rename(tmp_path, path);
        return PersistStatus::Ok;
    } catch (...) {
        // cleanup temp
        std::error_code ec;
        fs::remove(tmp_path, ec);
        return PersistStatus::IOError;
    }
}

std::optional<std::string> PersistenceLayer::read_string(const std::string& filename) const {
    auto bin = read_binary(filename);
    if (!bin) return std::nullopt;
    return std::string(bin->begin(), bin->end());
}

std::optional<std::vector<uint8_t>> PersistenceLayer::read_binary(const std::string& filename) const {
    std::lock_guard<std::mutex> lk(m_mutex);
    auto path = fs::path(m_base_dir) / filename;
    if (!fs::exists(path)) return std::nullopt;

    try {
        std::ifstream ifs(path, std::ios::binary);
        if (!ifs.is_open()) return std::nullopt;
        ifs.seekg(0, std::ios::end);
        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        if (size < 0) return std::nullopt;
        std::vector<uint8_t> buf(static_cast<size_t>(size));
        if (!ifs.read(reinterpret_cast<char*>(buf.data()), size)) {
            return std::nullopt;
        }
        return buf;
    } catch (...) {
        return std::nullopt;
    }
}

PersistStatus PersistenceLayer::remove(const std::string& filename) {
    std::lock_guard<std::mutex> lk(m_mutex);
    auto path = fs::path(m_base_dir) / filename;
    try {
        if (fs::exists(path)) {
            fs::remove(path);
            return PersistStatus::Ok;
        }
        return PersistStatus::NotFound;
    } catch (...) {
        return PersistStatus::IOError;
    }
}

bool PersistenceLayer::exists(const std::string& filename) const {
    try {
        return fs::exists(fs::path(m_base_dir) / filename);
    } catch (...) {
        return false;
    }
}

    }
}
