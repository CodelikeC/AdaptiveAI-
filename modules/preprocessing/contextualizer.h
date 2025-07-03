#ifndef contextualizer_h 
#define contextualizer_h 

#include <windows.h>
#include <iostream> 
using namespace std; 

#include <string> 
#include <map>
#include <vector>

class Contextualizer
{
    public: 
    Contextualizer(); 
    ~Contextualizer(); 

    string enrichWithContext(const string &data); 
    void loadSystemMetaData(const map<string, string> &metadata);
    
    private: 
    map<string, string> systemContext;
}; 
#endif 