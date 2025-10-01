#include "attack_scenario.h"
using namespace std; 
#include <thread>
#include <string> 
#include <chrono> 

#include <atomic> 

#include <functional> 

#include <random> 

namespace adaptive_ai 
{

using namespace std::chrono;

AttackScenario::AttackScenario(std::string id, std::string name)
    : m_id(std::move(id)), m_name(std::move(name)) {}

void AttackScenario::add_step(const AttackStep& step) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_steps.push_back(step);
}

bool AttackScenario::simulate(std::function<void(const ScenarioEvent&)> on_event,
                              std::function<void(bool)> on_complete) {
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_state == ScenarioState::Running) {
            // already running
            if (on_complete) on_complete(false);
            return false;
        }
        m_state = ScenarioState::Running;
        m_executed_steps.clear();
    }

    bool overall_success = true;

    for (const auto& step : m_steps) {
        // emit start event
        push_event(step.id, "step_started: " + step.description, on_event);

        // simulate work (blocking sleep)
        try {
            std::this_thread::sleep_for(step.duration);
            // simulate possible failure: here none; higher-level code can inject failures by overriding callbacks
            // mark executed
            {
                std::lock_guard<std::mutex> lk(m_mutex);
                m_executed_steps.push_back(step.id);
            }
            push_event(step.id, "step_completed", on_event);
        } catch (...) {
            push_event(step.id, "step_exception", on_event);
            overall_success = false;
            break;
        }
    }

    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_state = overall_success ? ScenarioState::Completed : ScenarioState::Failed;
    }

    if (on_complete) on_complete(overall_success);
    return overall_success;
}

bool AttackScenario::rollback(std::function<void(const ScenarioEvent&)> on_event) {
    std::lock_guard<std::mutex> lk(m_mutex);
    if (m_state != ScenarioState::Running && m_state != ScenarioState::Completed && m_state != ScenarioState::Failed) {
        // nothing to rollback
        return false;
    }

    bool all_ok = true;
    // rollback executed steps in reverse order
    for (auto it = m_executed_steps.rbegin(); it != m_executed_steps.rend(); ++it) {
        const std::string& sid = *it;
        // find step
        auto found = std::find_if(m_steps.begin(), m_steps.end(), [&](const AttackStep& s){ return s.id == sid; });
        if (found == m_steps.end()) continue;

        if (!found->reversible) {
            push_event(sid, "rollback_skipped_not_reversible", on_event);
            all_ok = false;
            continue;
        }

        // simulate rollback delay (we use half duration as example)
        push_event(sid, "rollback_started", on_event);
        std::this_thread::sleep_for(found->duration / 2);
        push_event(sid, "rollback_completed", on_event);
    }

    m_state = all_ok ? ScenarioState::RolledBack : ScenarioState::PartiallyRolledBack;
    m_executed_steps.clear();
    return all_ok;
}

std::string AttackScenario::id() const noexcept { return m_id; }
std::string AttackScenario::name() const noexcept { std::lock_guard<std::mutex> lk(m_mutex); return m_name; }
ScenarioState AttackScenario::state() const noexcept { std::lock_guard<std::mutex> lk(m_mutex); return m_state; }
std::vector<AttackStep> AttackScenario::steps() const noexcept { std::lock_guard<std::mutex> lk(m_mutex); return m_steps; }

void AttackScenario::push_event(const std::string& step_id, const std::string& msg,
                                std::function<void(const ScenarioEvent&)> on_event) const
{
    if (!on_event) return;
    ScenarioEvent ev;
    ev.scenario_id = m_id;
    ev.step_id = step_id;
    ev.message = msg;
    ev.timestamp = std::chrono::system_clock::now();
    try {
        on_event(ev);
    } catch (...) {
        // swallow exceptions from callbacks so simulation continues
    }
}

// simple serialization: format:
// id|name\nstep_id;desc;duration_ms;severity;reversible\n...
std::string AttackScenario::to_string() const {
    std::lock_guard<std::mutex> lk(m_mutex);
    std::ostringstream oss;
    oss << m_id << '|' << m_name << '\n';
    for (const auto& s : m_steps) {
        oss << s.id << ';'
            << s.description << ';'
            << s.duration.count() << ';'
            << s.severity << ';'
            << (s.reversible ? '1' : '0')
            << '\n';
    }
    return oss.str();
}

std::optional<AttackScenario> AttackScenario::from_string(const std::string& s) {
    std::istringstream iss(s);
    std::string header;
    if (!std::getline(iss, header)) return std::nullopt;
    auto pipe = header.find('|');
    if (pipe == std::string::npos) return std::nullopt;
    std::string id = header.substr(0, pipe);
    std::string name = header.substr(pipe+1);
    AttackScenario sc(id, name);

    std::string line;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        std::string token;
        std::vector<std::string> toks;
        while (std::getline(ls, token, ';')) toks.push_back(token);
        if (toks.size() < 5) continue;
        AttackStep st;
        st.id = toks[0];
        st.description = toks[1];
        try {
            st.duration = ms(std::stoll(toks[2]));
        } catch (...) { st.duration = ms(0); }
        try {
            st.severity = std::stoi(toks[3]);
        } catch (...) { st.severity = 0; }
        st.reversible = (toks[4] == "1");
        sc.add_step(st);
    }

    return;
}

} // namespace adaptive_ai
