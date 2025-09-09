#include <iostream> 
using namespace std; 

#include <thread>
#include <chrono>

#include "ipc.h"
#include "ipc.c"

// ham tu core.c va memory.c 

extern "C"
{
    void init_kernel();
    int spawn_process(const char *name, void *(entry_point)(void)); 
    void terminate_process(int pid); 
    void run_kernel(); 

    void memory_init(); 
    void *allocate_memory(int pid, size_t size); 
    void free_memory(int pid); 

    bool send_message(int to, char *buffer, int buffer_size); 
}

// ------------ Entry functions cho process ----------------- // 
void process_A()
{
    cout <<"[Process A] Hello from Process A!" << endl; 

    // Cap phat thu .. // 
    void *ptr = allocate_memory(1, 128); 
    if (ptr)
    {
        cout <<"[ProcessA] Allocated memory block!" << endl; 
    }
    send_message(1, 2, "Hello from A!") ; 
    this_thread :: sleep_for(chrono :: milliseconds(100));
}

void process_B()
{
    cout << "[Process B] - Hello from process B" << endl; 
    char buffer[128]; 

    if (receive_message(2, buffer, sizeof(buffer)))
    {
        cout << "[ProcessB] - Received message :" << buffer << endl; 
    }

    void *ptr = allocate_memory(2, 64);
    if (ptr){
        cout <<"[ProcessB] - Allocated memory block" << endl; 
    } 
    this_thread :: sleep_for(chrono :: microseconds(100)); 
}

// ----------------- Main test -------- // 
int main ()
{
    cout <<"======Core text start =======" << endl; 

    // Init kernel and memory .. // 
    init_kernel(); 
    memory_init();
    
    // Spawn processes .. // 
    // int pidA = spawn_process("ProcessA", process_A); 
    // int pidB = spawn_process("ProcessB", process_B);
    
    // cout <<"[Test] Spawned ProcessA with PID" << pidA << endl; 

    thread kernel_thread([]() {
        run_kernel();
    });

    // Cho chay mot chut roi terminate ...// 
    this_thread :: sleep_for(chrono :: seconds(1)); 
    
    
    return 0;
}