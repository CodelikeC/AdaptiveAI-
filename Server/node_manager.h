#ifndef NODE_MANAGER_H 
#define NODE_MANAGER_H 
#include <iostream> 
using namespace std; 

#include <vector> 
#include <mutex> 
#include "node.h"

class NodeManager 
{
    public:  
    NodeManager();
    ~NodeManager(); 

    void addNode(const Node &node); 
    void removeNode(const string &nodeID); 
    Node *getNode(const string &nodeID);
    vector<Node> getActiveNodes() const;  
    void updateNodeStatus(const string &nodeID, bool status); 

    private: 
    vector<Node> nodes_; 
    mutable mutex mutex_; 
};

#endif 

