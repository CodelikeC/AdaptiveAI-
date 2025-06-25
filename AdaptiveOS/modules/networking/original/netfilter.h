#ifndef NETFILTER_H 
#define NETFILTER_H 

#include <stdbool.h> 
#include <stdint.h>

#define MAX_PACKET_SIZE 2048 

typedef enum{
    PROTOCOL_UNKNOWN = 0, 
    PROTOCOL_TCP ,
    PROTOCOL_ICMP
} ProtocolType; 


typedef struct 
{
    char source_ip[16];
    char dest_ip[16];
    uint16_t source_port; 
    uint16_t dest_port; 
    ProtocolType protocol; 
    size_t payload_size; 
    char payload[MAX_PACKET_SIZE]; 
}Packet;

// Phan tich va xu ly goi tin .. // 
bool analyze_packet(Packet *pkt); 

// Kiem tra an ninh (basic filter) .. // 
bool is_mallicious(const Packet *pkt) ;

// trich xuat header .. // 
void extract_header(const char *raw_data, size_t len, Packet *out_pkt) ;

#endif 