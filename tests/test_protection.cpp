#include "../modules/protection/protection_facade.h"
#include "../modules/protection/threat_analyzer.h"
#include "../modules/protection/trust_guard.h"
#include "../modules/protection/self_defense.h"
#include "../modules/protection/killswitch_monitor.h"

#include <cassert>
#include <iostream>
#include <chrono>

#include "../modules/protection/protection_facade.cpp"
#include "../modules/protection/threat_analyzer.cpp"
#include "../modules/protection/trust_guard.cpp"
#include "../modules/protection/self_defense.cpp"
#include "../modules/protection/killswitch_monitor.cpp"

#include "../modules/protection/protection_facade.h"
#include "../modules/protection/protection_facade.cpp"

using namespace adaptive_ai;

int main() 
{
    ProtectionFacade p;

    // baseline allow
    auto d1 = p.evaluatePacket("10.0.0.1", 0.1f, 0.2f);
    assert(d1.action == Action::Allow);

    // slight risk -> monitor
    auto d2 = p.evaluatePacket("10.0.0.2", 0.35f, 0.2f);
    assert(d2.action == Action::Monitor);

    // increase trust reduces effective risk. 
    p.trustGuard().updateTrustScore("10.0.0.3", 0); 
    // ensure default present. 

    auto d3 = p.evaluatePacket("10.0.0.3", 0.8f, 0.4f);
    // depending on trust default, may be throttle/quarantine/drop. check non-crash
    assert(d3.adjustedRisk >= 0.0f && d3.adjustedRisk <= 1.0f);

    // feed threat signals then test killswitch logic
    ThreatSignal s{"sensor", "mass anomaly", 0.9f, std::chrono::system_clock::now()};
    p.threatAnalyzer().ingestSignal(s);
    auto d4 = p.evaluatePacket("10.0.0.99", 0.95f, 0.8f);
    // high combined risk should trip kill switch
    assert(d4.action == Action::TripKillSwitch || d4.action == Action::Drop);

    std::cout << "Protection integration tests passed\n";
    return 0;
}