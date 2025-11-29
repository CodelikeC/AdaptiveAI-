#include "base_protocol.h"
#include <string> 

#include <iostream>
using namespace std; 

#include <mutex> 
#include <vector> 

// COnnection 
// -- The communication endpoint of AICP>. 
// Can talk to : 
// + Runtime
// + Modules
// + External nodes(optional).. 

class Connector
{
    public: 
    explicit Connector(BaseProtocol *protocol); 
    ~Connector(); 

    // send a message through the protocol.. 
    bool send (const unordered_map<string, string> &msg );

    // receive raw buffer -> decode.. 
    unordered_map<string, string> receive(
        const vector<uint8_t> &raw
    );
    
    private: 
    BaseProtocol * proto; 
    mutex mtx; 
};