#pragma once 
#include <string> 

#include <vector> 
#include <cstdint>

#include <optional>
using namespace std; 

// protocol_utils.h 
// - Framing: add STX/ETX markers.. 
// - checksum : CRC 32 (IEEE)
// - serialization : string -> bytes..
// - message tagging. 
// - small helpers for safe narsing.. 

namespace protocol
{
    static const uint8_t STX = 0x02; 
    static const uint8_t ETX  = 0x03; 

    // -- Convert string -> bytes.. 
    vector<uint8_t> to_bytes(const string &s); 

    // --- Convert bytes -> string.. 
    string to_string(const vector<uint8_t> &bytes); 

    // CRC 32  - check sum..
    uint32_t crc32(const vector<uint8_t> &data); 
    uint32_t crc32(const uint8_t *data , size_t len); 

    // Frame a packet [STX][payload][CRC32][EXT].. 
    vector<uint8_t> frame_packet(const vector<uint8_t> &payload); 

    // Parse framed packet, return payload if valid.. 
    optional<vector<uint8_t>> deframe_packet(const vector<uint8_t> &raw); 

    // Safe - parsing tools.. 

    // find STX and ETX boundaries.. 
    optional<pair<size_t, size_t>> find_frame(const vector<uint8_t> &buf); 

    // extract CRC 32 at end of frame...
    uint32_t extract_crc(const vector<uint8_t> &raw, size_t etx_pos); 

    // remove <STX, ETX, CRC> and return pure payload.. 
    vector<uint8_t> extract_payload(const vector<uint8_t> &raw, size_t stx_pos, size_t etx_pos);

} // namespace protocol.. 