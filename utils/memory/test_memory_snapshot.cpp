#include "memory_snapshot.h"
#include "memory_snapshot.cpp"

using namespace std; 

using namespace adaptive_ai :: memory; 

int main ()
{
    MemorySnapshot snap1; 
    snap1.timestamp = chrono :: system_clock::now();
    snap1.process_rss = 1000000; 

    snap1.process_virtual = 2000000; 
    snap1.total_system = 8000000000;
    snap1.free_system = 4000000000; 
    snap1.tracked_allocs = 5 ; 
    snap1.tracked_bytes = 2040;
    
    cout << snap1.to_string() << endl; 

    string ser = snap1.serialize(); 
    cout <<"Serialized:" << ser << endl; 
    
    auto parsed = MemorySnapshot :: deserialize(ser); 
    if (parsed){
        cout << parsed -> to_string(false) << endl; 
    }

    MemorySnapshot snap2 = *parsed; 
    snap2.process_rss+= 5000; 
    cout << snap2.diff(snap1) << endl;
    
    return 0;
}