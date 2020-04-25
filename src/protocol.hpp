#pragma once
#include <cstdint>

namespace smartport {

enum FrameHeader : uint8_t {
    /**
     * Indicates a sensor packet. Packets with corresponding data IDs between
     * 0x5000 and 0x52ff are considered passthrough packets. Low IDs are
     * reserved for the official FrSky sensors.
     */
    sensor = 0x10,
    /**
     * Indicates a raw passthrough packet. OpenTX will ignore the data ID and
     * pass the packet directly to any Lua script.
     *
     * https://opentx.gitbooks.io/opentx-2-2-lua-reference-guide/content/general/sportTelemetryPop.html
     */
    passthrough = 0x32
};

/**
 * Data IDs defined here:
 * https://cdn.rawgit.com/ArduPilot/ardupilot_wiki/33cd0c2c/images/FrSky_Passthrough_protocol.xlsx
 */
enum DataID : uint16_t {
    text = 0x5000,
    ap_status = 0x5001,
    gps_status = 0x5002,
    battery = 0x5003,
    home = 0x5004,
    velocity_yaw = 0x5005,
    roll_pitch_range = 0x5006,
    params = 0x5007,

    // Application-defined IDs from here
    var_signature = 0x5100,

    // Data types
    var_int = var_signature,
    var_fixed = var_signature | 1u << 12,
    var_float,
    var_varint,
    var_string
};

}
