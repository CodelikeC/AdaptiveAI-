#include "lifecycle_manager.h"
#include "intergrity_checker.h"

#include <mutex>
#include <map>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "../utils/logger.h"

namespace adaptive_ai {
namespace core {

class PluginRecord {
public:
    std::string name;
    std::string path;
    bool loaded = false;
    // additional metadata could be added (version, signature, capabilities)
};

LifeCycleManager::LifeCycleManager()
{
    // default initialization
}

bool LifecycleManager::initialize() {
    std::lock_guard<std::mutex> l(mu_);
    utils::Logger::info("[LifecycleManager] initialize start");

    // 1) Basic integrity checks for known core files (for demo we check executable + a plugins folder)
    IntergrityChecker ic;
    std::vector<std::string> toCheck;

    // Example: check executable (this process) and plugins directory if exists
    try {
        // Attempt to find current executable path (best-effort)
        // We won't rely on platform-specific API; instead check working directory
        if (std::filesystem::exists("CMakeLists.txt")) {
            toCheck.push_back("CMakeLists.txt");
        }
        if (std::filesystem::exists("README.md")) {
            toCheck.push_back("README.md");
        }
        // Check plugins folder files (if any)
        if (std::filesystem::exists("plugin/")) {
            for (auto &p : std::filesystem::directory_iterator("plugin/")) {
                if (p.is_regular_file()) toCheck.push_back(p.path().string());
            }
        }
    } catch (const std::exception& ex) {
        utils::Logger::warn(std::string("[LifecycleManager] warning during init file discovery: ") + ex.what());
    }

    IntegrityReport rep = ic.validateAll(toCheck);
    if (!rep.ok) {
        utils::Logger::error("[LifecycleManager] integrity checks failed; entering safe state");
        state_ = LifecycleState::PAUSED;
        for (auto &cb : callbacks_) cb(state_);
        return false;
    }

    // load trust store if provided (configurable in future)
    ic.loadTrustStore("config/trust_store.pem");

    // Mark as running
    state_ = LifecycleState::RUNNING;
    Logger::info("[LifecycleManager] initialize completed, state=RUNNING");
    for (auto &cb : callbacks_) cb(state_);
    return true;
}

void LifecycleManager::onStateChange(const std::function<void(LifecycleState)>& cb) {
    std::lock_guard<std::mutex> l(mu_);
    callbacks_.push_back(cb);
}

bool LifecycleManager::loadPlugin(const std::string& path) {
    std::lock_guard<std::mutex> l(mu_);
    try {
        if (!std::filesystem::exists(path)) {
            utils::Logger::error("[LifecycleManager] loadPlugin: file not found " + path);
            return false;
        }

        // derive plugin name from filename
        std::string name = std::filesystem::path(path).stem().string();

        // If already registered, bail
        if (plugins_.count(name)) {
            utils::Logger::warn("[LifecycleManager] loadPlugin: plugin already registered " + name);
            return false;
        }

        PluginRecord rec;
        rec.name = name;
        rec.path = path;
        rec.loaded = true; // in real impl, we would dlopen / load symbol / instantiate

        plugins_.emplace(name, rec);

        utils::Logger::info("[LifecycleManager] loadPlugin: registered " + name + " from " + path);
        return true;
    } catch (const std::exception& ex) {
        utils::Logger::error(std::string("[LifecycleManager] loadPlugin exception: ") + ex.what());
        return false;
    }
}

bool LifecycleManager::unloadPlugin(const std::string& name) {
    std::lock_guard<std::mutex> l(mu_);
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        utils::Logger::warn("[LifecycleManager] unloadPlugin: not found " + name);
        return false;
    }

    // In real impl: call plugin shutdown hooks, free symbols, verify unload safe.
    plugins_.erase(it);
    utils::Logger::info("[LifecycleManager] unloadPlugin: removed " + name);
    return true;
}

bool LifecycleManager::reloadPlugin(const std::string& name) {
    std::lock_guard<std::mutex> l(mu_);
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        utils::Logger::warn("[LifecycleManager] reloadPlugin: not found " + name);
        return false;
    }
    // Simulate unload+load
    std::string path = it->second.path;
    plugins_.erase(it);
    // small pause to emulate reload
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    PluginRecord rec;
    rec.name = name;
    rec.path = path;
    rec.loaded = true;
    plugins_.emplace(name, rec);
    utils::Logger::info("[LifecycleManager] reloadPlugin: reloaded " + name);
    return true;
}

bool LifecycleManager::triggerRollback(int snapshotId) {
    std::lock_guard<std::mutex> l(mu_);
    // This method should coordinate with MemoryBridge and SandboxGuard; for now emit logs and update state.
    utils::Logger::warn("[LifecycleManager] triggerRollback called for snapshot " + std::to_string(snapshotId));

    // Set state to rolling back and notify subscribers
    state_ = LifecycleState::ROLLING_BACK;
    for (auto &cb : callbacks_) cb(state_);

    // Simulate some rollback activity (in real impl: call MemoryBridge.restoreSnapshot)
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // After simulated rollback, set to RUNNING
    state_ = LifecycleState::RUNNING;
    for (auto &cb : callbacks_) cb(state_);
    utils::Logger::info("[LifecycleManager] rollback completed for snapshot " + std::to_string(snapshotId));
    return true;
}

LifecycleState LifecycleManager::state() const {
    std::lock_guard<std::mutex> l(mu_);
    return state_;
}

void LifecycleManager::shutdown() {
    std::lock_guard<std::mutex> l(mu_);
    utils::Logger::info("[LifecycleManager] shutdown requested");
    state_ = LifecycleState::SHUTTING_DOWN;
    for (auto &cb : callbacks_) cb(state_);

    // unload plugins gracefully
    std::vector<std::string> names;
    for (const auto &p : plugins_) names.push_back(p.first);
    for (const auto &n : names) {
        // attempt unload; ignore failures
        unloadPlugin(n);
    }

    // finalize
    state_ = LifecycleState::PAUSED;
    for (auto &cb : callbacks_) cb(state_);
    utils::Logger::info("[LifecycleManager] shutdown completed, state=PAUSED");
}

} // namespace core
} // namespace adaptive_ai
