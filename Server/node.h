#ifndef NODE_H
#define NODE_H 

#include <iostream> 
using namespace std; 

#include <string>
#include <vector>

class Node 
{
    public: 
    Node(const string &id, const string &ip, int port);
    ~Node(){}; 
    
    string getID() const 
    {
        return id_;
    }
    string getIP() const 
    {
        return ip_; 
    }

    int getPort() const 
    {
        return port_; 
    }

    bool isActive() const 
    {
        return active_; 
    }

    void setActive(bool status)
    {
        active_ = status; 
    }

    private: 
    string ip_;
    string id_; 
    bool active_; 
    int port_; 

};


#endif 

