#include "sync_manager.h"
using namespace std; 

#include <iostream> 
#include <thread> 

#include <chrono>
#include <unordered_map>

#include <mutex> 
#include <sstream> 

#include <random> 
using namespace adaptive_ai :: net; 

namespace {
    struct RemoteNode {
        string address;
        bool online = true;
    };

    struct NetworkState {
        unordered_map<string, RemoteNode> nodes;
        function<void(int, const string&)> snapshotCallback;
        bool running = false;
        string selfAddr;
        mutex mu;
    };

    NetworkState g_net;

    SyncManager::SyncManager() {}

bool SyncManager::start(const string &bindAddr) {
    lock_guard<mutex> lock(g_net.mu);

    if (g_net.running) {
        cerr << "[SyncManager] Already running on " << g_net.selfAddr << endl;
        return false;
    }

    g_net.running = true;
    g_net.selfAddr = bindAddr;
    g_net.nodes[bindAddr] = { bindAddr, true };

    cout << "[SyncManager] Node started at " << bindAddr << endl;
    return true;
}

void SyncManager::stop() {
    lock_guard<mutex> lock(g_net.mu);

    if (!g_net.running) {
        cerr << "[SyncManager] Not running." << endl;
        return;
    }

    cout << "[SyncManager] Stopping node " << g_net.selfAddr << endl;
    g_net.running = false;
    g_net.nodes.erase(g_net.selfAddr);
    g_net.selfAddr.clear();
}

bool SyncManager::pushSnapshot(const string &nodeAddr, int snapshotId) {
    lock_guard<mutex> lock(g_net.mu);

    if (!g_net.running) {
        cerr << "[SyncManager] Cannot push, node is offline." << endl;
        return false;
    }

    auto it = g_net.nodes.find(nodeAddr);
    if (it == g_net.nodes.end() || !it->second.online) {
        cerr << "[SyncManager] Target node not found or offline: " << nodeAddr << endl;
        return false;
    }

    cout << "[SyncManager] Pushing snapshot #" << snapshotId
         << " to node " << nodeAddr << "..." << endl;

    // Giả lập delay mạng
    thread([snapshotId, nodeAddr]() {
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 200));

        string payload = "Snapshot_" + to_string(snapshotId) + "_payload";
        cout << "[SyncManager] Snapshot #" << snapshotId
             << " delivered to " << nodeAddr << endl;

        // Nếu node nhận có callback → gọi callback
        lock_guard<mutex> cbLock(g_net.mu);
        if (g_net.snapshotCallback)
            g_net.snapshotCallback(snapshotId, payload);
    }).detach();

    return true;
}

bool SyncManager::requestSnapshot(const string &nodeAddr, int snapshotId) {
    lock_guard<mutex> lock(g_net.mu);

    if (!g_net.running) {
        cerr << "[SyncManager] Cannot request, node is offline." << endl;
        return false;
    }

    auto it = g_net.nodes.find(nodeAddr);
    if (it == g_net.nodes.end() || !it->second.online) {
        cerr << "[SyncManager] Source node not found: " << nodeAddr << endl;
        return false;
    }

    cout << "[SyncManager] Requesting snapshot #" << snapshotId
         << " from " << nodeAddr << "..." << endl;

    // Giả lập async pull
    thread([snapshotId, nodeAddr]() {
        this_thread::sleep_for(chrono::milliseconds(80 + rand() % 200));

        string payload = "Fetched_Snapshot_" + to_string(snapshotId) + "_from_" + nodeAddr;
        cout << "[SyncManager] Snapshot #" << snapshotId
             << " received from " << nodeAddr << endl;

        lock_guard<mutex> cbLock(g_net.mu);
        if (g_net.snapshotCallback)
            g_net.snapshotCallback(snapshotId, payload);
    }).detach();

    return true;
}

void SyncManager::onSnapshotReceive(const function<void(int, const string &)> &cb) 
{
    lock_guard<mutex> lock(g_net.mu);
    g_net.snapshotCallback = cb;
    cout << "[SyncManager] Snapshot receive callback registered." << endl;
}
}