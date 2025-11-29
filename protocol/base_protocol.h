#pragma once 
#include <string> 
#include <unordered_map>

#include <vector> 
#include <iostream>
#include <vector> 

using namespace std; 

// base protocol.. 
// Abstract interface for excoding/ decoding AICP protocol.. 
// All protocol implementation must follow this interface.. 

class BaseProtocol
{
    public: 
    virtual ~BaseProtocol() = default; 

    // Encode a structured message into raw bytes... // 
    virtual vector<uint8_t> encode(
        const unordered_map<string, string> &msg
    ) = 0;

    // decode raw bytes into message dictionary.. 
    virtual unordered_map<string, string> decode
    (
        const vector<uint8_t> &raw
    ) = 0;
    
    // Get protocol version (for manifest..)
    virtual string get_version() const = 0; 

    // Validate message structure BEFORE encoding..
    virtual bool validate(
        const unordered_map<string, string> &msg
    ) = 0; 
}; 
