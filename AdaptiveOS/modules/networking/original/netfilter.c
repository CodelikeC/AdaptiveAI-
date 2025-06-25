#include "netfilter.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

bool is_mallicious(const Packet *pkt)
{
    if (strstr(pkt -> payload , "virus") || pkt -> payload_size)
    {
        return 1; 
    }
    return 0 ; 
}

bool analyze_packet(Packet *pkt)
{
    printf("[NetFilter] Analyzing packet from %s : %d to %s : %d (protocol : %d, size : %zu)\n"); 
    printf(pkt -> source_ip, pkt -> source_port, pkt -> dest_ip, pkt -> dest_port, pkt ->payload_size); 
    if (is_mallicious(pkt))
    {
        printf("[Netfilter] Dropped malicious packet!\n"); 
        return 0; 
    }

    // neu hop le -> chuyen den he thong xu ly 
    printf("[netfilter] Packet accepted!\n");
    return 1; 
}

void extract_header(const char *raw_data, size_t len, Packet *out_pkt)
{
    if (len < 20) return; // Gia dinh toi thieu.. // 
    // gia lap parsing : phan dau tien chua IP / port ..// 
    snprintf(out_pkt-> source_ip,16, "192.168.1.10"); 
    snprintf(out_pkt-> dest_ip, 16, "192.168.1.20");  
    out_pkt -> source_port = 12345; 
    out_pkt -> dest_port = 80; 
    out_pkt -> protocol = PROTOCOL_TCP; 

    out_pkt -> payload_size = len - 20; 
    memcpy(out_pkt-> payload, raw_data + 20, out_pkt -> payload_size); 
}