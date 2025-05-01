#include <iostream>
using namespace std;

#include <string> 
#include <map> 
#include <vector> 

namespace adaptive_ai
{
    class NetworkSensor
    {
        public: 
        struct packet_info
        {
            string src_ip;
            string dest_ip; 
            int src_port; 
            int dest_port; 
            string protocol; 
            size_t size; 
            long timestamp  ;
        }; 

        NetworkSensor(); 
        ~NetworkSensor();

        void capture_packet(const packet_info &packet);
        vector<packet_info> get_recent_packets(size_t max_packets = 100);
        map<string, int> summarize_protocol_usage() const; 
        
        private : 
        vector<packet_info> packet_buffer; 
        size_t max_buffer_size = 100; 
    }; 
}