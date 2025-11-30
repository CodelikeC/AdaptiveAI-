#pragma once 
#include <string>

#include <unordered_map>
#include <vector>

using namespace std; 

class SchemaValidator
{
    public: 
    struct Error
    {
        bool ok; 
        string message; 
    }; 

    explicit SchemaValidator(const string &schema_json); 

    // Validate a message.. 
    Error validate(const unordered_map<string, string> &msg);
    
    private: 
    // loaded schema (type -> required fields).. 
    unordered_map<string, vector<string>> required_fields;
    
    bool parse_schema(const string &json); 
}; 