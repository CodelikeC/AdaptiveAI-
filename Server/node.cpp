#include <iostream> 
#include <string> 
#include <algorithm>

#include "node.h"

Node :: Node(const string &id, const string &ip, int port)
: id_(id), ip_(ip), port_(port) , active_(true){}
