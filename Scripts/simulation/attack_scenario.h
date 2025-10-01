#pragma once 
#include <iostream> 
#include <vector>
#include <string> 

using namespace std; 
#include <functional> 
#include <mutex> 
#include <optional> 

namespace adaptive_ai 
{
    using ms = chrono :: milliseconds; 

    struct AttackStep{
        string id; 
        // id cho step ..//
        string description; 
        // Mo ta cho step ..// 
        ms duration; 
        // do dai step ..// 
        function<bool()> actions; 
        int severity{0};
        // muc do nghiem trong ..// 
        bool reversible{true}; 

        AttackStep() = default; 
        AttackStep(string iid, string desc, ms d, int sev, bool rev = true 
        ) : id(std::move(iid)), description(std::move(desc)), duration(d), severity(sev), reversible(rev) {}
    }; 
    // Trang thai scenario trong luc mo phong..// 
    enum class ScenarioState{
        Idle, 
        Running, 
        PartiallyRolledBack, 
        RolledBack,
        Completed, 
        Failed 
    };
    // Event structure - co the mapping sang Json ben ngoai.. // 
    struct ScenarioEvent
    {
        string scenario_id; 
        string step_id; 
        string message; 
        chrono :: system_clock :: time_point timestamp; 
    };

    // AttackScenario Class .. // 
    class AttackScenario 
    {
        public: 
        AttackScenario(string id, string name); 

        // Them step .. // 
        void add_step(const AttackStep &step); 

        bool simulate(function<void(const ScenarioEvent&)> on_event = nullptr, 
        function<void(bool)> on_complete = nullptr);

        // Rollback cac step da duoc thuc hien ..// 
        // returns true neu rollback thanh cong...// 
        bool rollback(function<void(const ScenarioEvent&)> on_event = nullptr); 

        // Getters ..// 
        string id() const noexcept; 
        string name() const noexcept; 
        ScenarioState state() const noexcept; 
        vector<AttackStep> steps() const noexcept; 

        string to_string()const; 
        static optional<AttackScenario> from_string(const string &s);
        private: 
        const string m_id; 
        string m_name;
        vector<AttackStep>m_steps; 

        /// Cac step da hoan thanh (id list) de rollback .. 
        vector<string> m_executed_steps; 

        mutable mutex m_mutex;
        ScenarioState m_state{
            ScenarioState :: Idle
        }; 

        // Helper internal..//
        void push_event(const string &step_id, const string &msg, 
        function<void(const ScenarioEvent &)> on_event) const;
        void set_state(ScenarioState new_state, function<void(bool)> on_complete = nullptr);

    };
}



