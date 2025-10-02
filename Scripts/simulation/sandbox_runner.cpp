// sandbox_runner.cpp
#include "sandbox_runner.h"
#include <algorithm>
#include <iostream>
#include <thread>

namespace adaptive_ai {

SandboxRunner::SandboxRunner(std::function<void(const ScenarioEvent&)> event_cb, size_t max_concurrency)
    : m_event_cb(std::move(event_cb)), m_max_concurrency(std::max<size_t>(1, max_concurrency))
{}

SandboxRunner::~SandboxRunner() {
    // best-effort stop (no harsh cancellation of running threads)
    stop();
}

void SandboxRunner::add_scenario(std::shared_ptr<AttackScenario> scenario) {
    if (!scenario) return;
    std::lock_guard<std::mutex> lk(m_mutex);
    m_scenarios.push_back(std::move(scenario));
}

size_t SandboxRunner::scenario_count() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_scenarios.size();
}

void SandboxRunner::stop() {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_stopped = true;
}

std::vector<SandboxResult> SandboxRunner::run_all(std::chrono::milliseconds per_scenario_timeout) {
    std::vector<std::shared_ptr<AttackScenario>> list;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        list = m_scenarios;
    }

    std::vector<SandboxResult> results;
    results.reserve(list.size());

    // simple concurrency control using a vector of futures and a semaphore-like counter
    std::vector<std::future<SandboxResult>> futures;
    futures.reserve(list.size());

    size_t next = 0;
    // launch initial batch
    for (; next < list.size() && futures.size() < m_max_concurrency; ++next) {
        auto sc = list[next];
        futures.emplace_back(std::async(std::launch::async, &SandboxRunner::run_with_monitor, this, sc, per_scenario_timeout));
    }

    // whenever a future finishes, collect result and launch next
    while (!futures.empty()) {
        // wait for any to finish (polling)
        for (auto it = futures.begin(); it != futures.end(); ) {
            auto &f = *it;
            using namespace std::chrono_literals;
            if (f.wait_for(50ms) == std::future_status::ready) {
                try {
                    results.push_back(f.get());
                } catch (...) {
                    SandboxResult r;
                    r.scenario_id = "<unknown>";
                    r.success = false;
                    r.message = "exception in future";
                    results.push_back(r);
                }
                it = futures.erase(it);
                // launch next if any
                if (next < list.size() && !m_stopped) {
                    auto sc = list[next++];
                    futures.emplace_back(std::async(std::launch::async, &SandboxRunner::run_with_monitor, this, sc, per_scenario_timeout));
                }
            } else {
                ++it;
            }
        }
        if (m_stopped) {
            // if stopped, wait for currently running to finish but don't start new ones (we already prevented new ones)
            // continue loop until futures empty
            continue;
        }
        // small sleep to avoid busy loop
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return results;
}

std::optional<SandboxResult> SandboxRunner::run_one(const std::string& scenario_id, std::chrono::milliseconds timeout) {
    std::shared_ptr<AttackScenario> sc;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        auto it = std::find_if(m_scenarios.begin(), m_scenarios.end(), [&](const std::shared_ptr<AttackScenario>& s){
            return s && s->id() == scenario_id;
        });
        if (it == m_scenarios.end()) return std::nullopt;
        sc = *it;
    }
    if (!sc) return std::nullopt;
    return run_with_monitor(sc, timeout);
}

SandboxResult SandboxRunner::run_with_monitor(std::shared_ptr<AttackScenario> sc, std::chrono::milliseconds timeout) {
    SandboxResult result;
    if (!sc) {
        result.message = "null scenario";
        return result;
    }
    result.scenario_id = sc->id();

    // forward events from scenario to runner's event_cb; we also augment messages for tracing
    auto event_forwarder = [this](const ScenarioEvent& ev){
        // possible place to enrich/transform event
        if (m_event_cb) {
            try { m_event_cb(ev); } catch(...) { /* swallow */ }
        }
    };

    auto start_tp = std::chrono::steady_clock::now();

    // run simulate in async so we can timeout
    auto fut = std::async(std::launch::async, [sc, &event_forwarder]() -> bool {
        // call simulate; capture success
        try {
            bool ok = sc->simulate(event_forwarder, nullptr);
            return ok;
        } catch (...) {
            // treat as failure
            return false;
        }
    });

    bool finished = false;
    if (timeout.count() > 0) {
        if (fut.wait_for(timeout) == std::future_status::ready) {
            finished = true;
            try {
                result.success = fut.get();
            } catch (...) {
                result.success = false;
            }
        } else {
            // timeout occurred
            result.timed_out = true;
            result.success = false;
        }
    } else {
        // no timeout: wait indefinitely
        try {
            result.success = fut.get();
            finished = true;
        } catch (...) {
            result.success = false;
            finished = true;
        }
    }

    auto end_tp = std::chrono::steady_clock::now();
    result.ran_for = std::chrono::duration_cast<std::chrono::milliseconds>(end_tp - start_tp);

    // If failed or timed out, attempt rollback
    if (!result.success) {
        // attempt rollback; we forward events too
        try {
            bool rb_ok = sc->rollback(event_forwarder);
            result.rolled_back = rb_ok;
            if (result.timed_out) {
                result.message = rb_ok ? "timed out, rolled back" : "timed out, partial/no rollback";
            } else {
                result.message = rb_ok ? "failed, rolled back" : "failed, partial/no rollback";
            }
        } catch (...) {
            result.rolled_back = false;
            result.message = "rollback raised exception";
        }
    } else {
        result.message = "completed";
    }

    return result;
}

} // namespace adaptive_ai
