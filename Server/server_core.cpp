#include "server_core.h"
using namespace std ; 

ServerCore :: ServerCore(const string &nodeId, const string &ip , int port)
: localNode_(nodeId, ip, port), sync_(NodeManager_), p2p_(localNode_){}

ServerCore :: ~ServerCore(){}

void ServerCore :: start()
{
    NodeManager_.addNode(localNode_); 
    sync_.startSync(); 
    cout <<"Server Started on" << localNode_.getIP() << endl; 
}

void ServerCore :: stop()
{
    sync_.stopSync(); 
    cout <<"Server Stopped" << endl; 
}

void ServerCore :: processData(const string &data)
{
    sync_.syncData(localNode_.getID(), data); 
    // co the them logic AI phan tich du lieu .. . 
    auto hitlCallback = [this](const std::string& issue) 
    {
        cout << "HITL Request: " << issue << endl;
        // Giả lập phản hồi HITL (trong thực tế, cần giao diện người dùng)
        handleHITLResponse("approve");
    }; 

    adaptiveAi_.analyzeData(data, state, hitlCallback);
    std::cout << "Analysis Result: " << adaptiveAi_.getAnalysisResult() << std::endl;

    // Xử lý đặc biệt cho từng trạng thái
    switch (state) {
        case AdaptiveAI::State::BACKUP: 
        {
            // So sánh với AWS/Azure: Giải pháp phi tập trung hiệu quả hơn
            auto activeNodes = NodeManager_.getActiveNodes();
            for (const auto& node : activeNodes) 
            {
                if (node.getID() != localNode_.getID()) 
                {
                    p2p_.sendMessage(node.getID(), node.getPort(), data);
                }
            }
            cout << "Backup mode: Data distributed to " << activeNodes.size() - 1 << " nodes." << endl;
            break;
        }
        case AdaptiveAI::State::DB_OVERHEATING:
            cooldownDatabase();
            break;
        case AdaptiveAI::State::SELF_REPAIR:
            expandServer("external_system_1");
            break;
        case AdaptiveAI::State::CORRUPTED_DATA:
            if (hitlApprovedSelfDestruct_) 
            {
                cout << "Self-destruct initiated." << endl;
                stop();
            }
            break;
        default:
            break;
    }
}

void ServerCore::handleHITLResponse(const std::string& response) 
{
    if (response == "approve") 
    {
        hitlApprovedSelfDestruct_ = true;
    } 
    else 
    {
        hitlApprovedSelfDestruct_ = false;
    }
}

void ServerCore::cooldownDatabase() 
{
    cout << "Cooling down database: Reducing load..." << endl;
    // Giả lập giảm tải nhiệt (trong thực tế, có thể giảm CPU usage hoặc tắt tiến trình)
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Database cooled down." << endl;
}

void ServerCore::expandServer(const string& externalSystem) 
{
    adaptiveAi_.expandServer(externalSystem);
}
