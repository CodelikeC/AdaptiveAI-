#include "channel_factory.h"

namespace communication
{
    ChannelFactory :: ChannelFactory()
    {
        cout <<"Channel Factory is working" << endl; 
    }

    ChannelFactory :: ~ChannelFactory()
    {
        cout << endl; 
    }

    unique_ptr<SecureChannel> ChannelFactory::create_secure(string& address, int port) 
    {
    return std::make_unique<SecureChannel>(address, port);
    }

    unique_ptr<RpcClient> ChannelFactory::create_rpc(string& address, int port) 
    {
    return std::make_unique<RpcClient>(address, port);
    }
}