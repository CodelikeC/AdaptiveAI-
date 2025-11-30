#include "protocol_utils.h"
#include <stdexcept>

#include <iostream>
using namespace std; 

namespace protocol
{
    vector<uint8_t> to_bytes(const string &s)
    {
        return vector<uint8_t> (s.begin(), s.end()); 
    }

    string to_string(const vector<uint8_t> &bytes)
    {
        return string(bytes.begin(), bytes.end()); 
    }

    // ====== CRC32 (IEEE) ====== // 
    static uint32_t crc_table[256]; 
    static bool crc_init_done = false; 

    static void init_crc32()
    {
        if (crc_init_done) return ; 
        for (uint32_t i = 0 ; i < 256 ; i++)
        {
            uint32_t c = i; 
            for (int j = 0 ; j < 8 ; j++)
            c = (c & 1) ? (0xEDB88320UL ^ (c >> 1)) : (c >> 1); 
            crc_table[i] = c; 
        }
        crc_init_done = true ; 
    }

    uint32_t crc32(const uint8_t *data, size_t len)
    {
        init_crc32(); 
        uint32_t c = 0xFFFFFFFFUL; 

        for (size_t i = 0; i < len; i++)
        c = crc_table[(c ^ data[i]) &0xFF] ^(c>>8); 
        return c ^ 0xFFFFFFFFUL; 
    }

    uint32_t crc32(const vector<uint8_t> &data)
    {
        return crc32(data.data(), data.size()); 
    }

    // --- Packet framing : [STX][payload][CRC32 4 bytes][ETX].. 
    vector<uint8_t> frame_packet(const vector<uint8_t> &payload)
    {
        vector<uint8_t> out; 
        out.reserve(payload.size() + 6); 

        out.push_back(STX); 

        for (auto &b : payload) out.push_back(b); 

        uint32_t checksum = crc32(payload); 
        out.push_back((checksum >> 24) & 0xFF); 
        out.push_back((checksum >> 16) & 0xFF);
        out.push_back((checksum >> 8) & 0xFF);
        out.push_back((checksum >> 0) & 0xFF); 

        out.push_back(ETX); 
        return out; 
    }

    // --- Deframe packet.. --- // 
    optional<vector<uint8_t>> deframe_packet(const vector<uint8_t> &raw){
        auto rng  = find_frame(raw); 
        if (!rng) return nullopt; 

        auto [stx_pos, etx_pos] = rng.value(); 
        if (etx_pos <= stx_pos + 5) return nullopt; 

        uint32_t recv_crc = extract_crc(raw, etx_pos); 
        vector<uint8_t> payload = extract_payload(raw, stx_pos, etx_pos); 
        uint32_t calc_crc = crc32(payload); 

        if (recv_crc != calc_crc) return nullopt; 

        return payload; 
    }

    optional<pair<size_t, size_t>> find_frame(const vector<uint8_t>&buf) 
    {
        size_t stx_pos = buf.size(); 
        size_t etx_pos = buf.size(); 

        for (size_t i = 0; i < buf.size(); i++){
            if (buf[i] == STX){
                stx_pos == i;
                break;  
            }
        }
        if (stx_pos == buf.size()) return nullopt;
        for (size_t i = stx_pos + 1; i < buf.size(); i++){
            if (buf[i] == ETX) etx_pos = i; break; 
        }
        if (etx_pos == buf.size()) return nullopt; 
        return make_pair(stx_pos, etx_pos); 
    }

    uint32_t extract_crc(const vector<uint8_t>& raw, size_t etx_pos) 
    {
    if (etx_pos < 4) return 0;

    size_t crc_pos = etx_pos - 4;

    return (raw[crc_pos] << 24) |
           (raw[crc_pos + 1] << 16) |
           (raw[crc_pos + 2] << 8) |
           (raw[crc_pos + 3] << 0);
    }

    vector<uint8_t> extract_payload(const vector<uint8_t>& raw, size_t stx_pos, size_t etx_pos) 
    {
        size_t begin = stx_pos + 1;
        size_t end = etx_pos - 4;
        return vector<uint8_t>(raw.begin() + begin, raw.begin() + end);
    }

} // namespace protocol.. 