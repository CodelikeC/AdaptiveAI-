#include <iostream> 
using namespace std; 
#include "network_sensor.h"
#include "../utils/logger.h"
#include <random> 
#include <chrono> 

using namespace adaptive_ai ;

// using packet_info = NetworkSensor::packet_info;

namespace adaptive_ai
{
    NetworkSensor :: NetworkSensor()
    {
        packet_buffer.reserve(max_buffer_size); 
        cout <<"Network sensor is:" << max_buffer_size << endl;
    }
    NetworkSensor :: ~NetworkSensor()
    {
        packet_buffer.clear(); 
        cout <<"packet buffer destroyed!" << endl;
    }
    // capture a packet and store it in the buffer.. // 
    void NetworkSensor::capture_packet(const struct packet_info &packet)
    {
        vector<packet_info> packet_buffer; 
        size_t max_buffer_size = 100; 
        // if buffer is full, remove the folder oldest packets .. 
        if (packet_buffer.size() >=max_buffer_size )
        {
            packet_buffer.erase(packet_buffer.begin()); 
        }
        // add the new packet to the buffer // 
        packet_buffer.push_back(packet); 
        // std::cout << "Captured packet: " <<packet.src_ip << " -> " << packet.dest_ip 
        //           << " (Protocol: " << packet.protocol << ", Size: " << packet.size << " bytes)" 
        //           << std::endl;
        Logger::debug("Captured packet from ", packet.src_ip, " to ", packet.dest_ip, 
            " (Protocol: ", packet.protocol, ", Size: ", packet.size, ")");         
    }

    // Get recent packets (up to max_packets)
    // vector<packet_info> get_recent_packets(size_t max_packets = 100) 
    // {
    //     // If max_packets is 0 or greater than buffer size, adjust to buffer size
    //     size_t num_packets = min(max_packets, packet_buffer.size());
    //     // Return the most recent packets (from the end of the buffer)
    //     return std::vector<packet_info>(packet_buffer.end() - num_packets, packet_buffer.end());
    // }
vector<NetworkSensor::packet_info> NetworkSensor::get_recent_packets(size_t max_packets)
{
    size_t count = min(packet_buffer.size(), max_packets);
    return vector<packet_info>(packet_buffer.end() - count, packet_buffer.end());
}

map<string, int> NetworkSensor::summarize_protocol_usage() const
{
    map<string, int> protocol_count;
    for (const auto &pkt : packet_buffer)
    {
        protocol_count[pkt.protocol]++;
    }

    Logger::info("Protocol usage summary:");
    for (const auto &[protocol, count] : protocol_count)
    {
        Logger::info("  ", protocol, ": ", count);
    }

    return protocol_count;
}

}