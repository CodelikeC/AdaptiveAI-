#include "communication_manager.h"

namespace communication 
{
    CommunicationManager :: CommunicationManager(const string &address, int port)
    : rpc_client(address, port), secure_channel(address, port){}

    CommunicationManager :: CommunicationManager()
    {
        cout <<"The communication is running" << endl ; 
        initialize();
    }
    bool CommunicationManager :: initialize()
    {
        bool rpc_ready = rpc_client.ping_server(); 
        bool secure_ready = secure_channel.establish_connection() ; 
        return rpc_ready && secure_ready; 
    }

    CommunicationManager :: ~CommunicationManager()
    {
        cout << "[CommunicationManager] Destructor called. Cleaning up..." << endl;
        shutdown();
    }

    string CommunicationManager :: transmit(const string &request)
    {
        if (!secure_channel.connected_)
        {
            return rpc_client.send_request(request); // fallback
        }
        secure_channel.send_secure(request);
        return secure_channel.receive_secure(); 
    }

    void CommunicationManager :: shutdown()
    {
        secure_channel.close();
    }
}
