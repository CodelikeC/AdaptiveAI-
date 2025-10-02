#pragma once 
#include "attack_scenario.h"
#include <string> 
#include <vector> 
#include <memory> 
#include <functional>
#include <map> 
#include <mutex> 
#include <unordered_map>
#include <future>
#include <chrono>
#include <optional> 

using namespace std; 

namespace adaptive_ai 
{ 
    using ms = chrono :: milliseconds; 

    // Ket qua dang chay sandbox..//
    struct SandboxRunResult
    {
        string scenario_id;
        bool success{false}; 
        bool timed_out{false}; 
        bool rolled_back{false};
        string message; 
        chrono:: milliseconds ran_for{0};
    };

    class SandboxRunner
    {
        public: 
        // Event cb : callback de forward ScenarioEvent (vi du gui vao comm_channel)
        // max_concurrency : so luong scenario chay dong thoi toi da .. // 
        SandboxRunner(function<void(const ScenarioEvent&)> event_cb = nullptr , size_t max_concurrency = 4);
        
        ~SandboxRunner(); 

        // Them scenario (khong copy, dung shared_ptr)
        void add_scenario(shared_ptr<AttackScenario> scenario); 

        // Chay tat ca scenario - returns list of results in the same order as added
        // per scenartio_timeout : timeout mac dinh cho moi scenario (ms), Neu <= 0 thi khog timeout.. //
        vector<SandboxRunResult> run_all(chrono::milliseconds per_scenario_timeout);
        // Chay mot scenario theo id (blocking)
        optional<SandboxRunResult> run_one(const string &scenario_id, chrono :: milliseconds timeout = chrono :: milliseconds(0));

        // Dung moi tac vu dang cho(khong stop thread dang chay simulate , chi prevents new runs . .)
        void stop(); 

        // Getters .. // 
        size_t scenario_count() const; 

        protected: 
        // Helper de chay 1 scenario voi timeout va rollback logic .. // 
        SandboxRunResult run_with_result(shared_ptr<AttackScenario> sc, chrono :: milliseconds timeout); 

        // Luu giu order .. // 
        vector<shared_ptr<AttackScenario>> m_scenarios;
        mutable mutex m_mutex; 

        function<void(const ScenarioEvent&) > m_event_cb ;
        size_t m_max_concurrency{4}; 
        bool m_stopped{false}; 

    };
}   
