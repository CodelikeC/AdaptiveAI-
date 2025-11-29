#include "connector.h"
#include <iostream>
using namespace std;

Connector :: Connector(BaseProtocol *protocol): proto(proto){}

Connector :: ~Connector(){}

bool Connector :: send(const unordered_map<string, string> &msg)
{
    lock_guard <mutex> lock(mtx); 
    if (!proto -> validate(msg)){
        cout << "AICP protocol validation failed:" << endl; 
        return false; 
    }

    vector <uint8_t> encoded = proto -> encode(msg); 

    // Here must send through real transport layer.. 
    // For now we simulate the sending.. 
    cout <<"AICP Sending packet" << encoded.size() << endl;
    return true; 
}

unordered_map<string, string> Connector :: receive(const vector<uint8_t> &raw){
    lock_guard<mutex> lock(mtx); 
    return proto -> decode(raw); 
}