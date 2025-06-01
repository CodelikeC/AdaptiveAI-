#include "node_manager.h"
#include "node.h"
using namespace std; 

NodeManager :: NodeManager(){}
NodeManager :: ~NodeManager(){}

void NodeManager :: addNode(const Node &node)
{
    lock_guard<mutex> lock(mutex_);  
    nodes_.push_back(node); 
}

void NodeManager :: removeNode(const string &nodeId)
{
    lock_guard<mutex> lock(mutex_); 
    nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(),
        [&nodeId](const Node& n) { 
            return n.getID() == nodeId; }),
            nodes_.end());

}

Node*NodeManager :: getNode(const string &nodeId)
{
    lock_guard<mutex> lock(mutex_); 
    for (auto &node : nodes_)
    {
        if (node.getID() == nodeId)
        return &node;
    }
    return nullptr; 
}

vector<Node> NodeManager :: getActiveNodes() const 
{
    lock_guard<mutex> lock(mutex_); 
    vector<Node> activeNodes; 
    for (const auto &node : nodes_)
    {
        if (node.isActive())
        activeNodes.push_back(node); 
    }
    return activeNodes; 
}

void NodeManager :: updateNodeStatus(const string &nodeId, bool status)
{
    lock_guard<mutex> lock(mutex_); 
    if (Node * node = getNode(nodeId))
    {
        node -> setActive(status); 
    }   
}