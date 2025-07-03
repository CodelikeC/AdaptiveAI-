#include <iostream> 
using namespace std; 

#include "contextualizer.h"

Contextualizer :: Contextualizer(){}
Contextualizer :: ~Contextualizer(){}

void Contextualizer :: loadSystemMetaData(const map<string, string> &metadata)
{
    systemContext = metadata; 
}

string Contextualizer::enrichWithContext(const string& data) 
{
    string result = data;
    for (const auto& [key, value] : systemContext) 
    {
        result += " | " + key + ": " + value;
    }
    return result;
}