#pragma once
#include <cstdint>
#include <iostream>

namespace smartport {

enum FrameHeader {
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
enum DataID {
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
    var_int = 0,
    var_fixed = 1u << 12,
    var_float,
    var_varint,
    var_string
};

// TODO: in 32 bits, stuff as much data into the packet as possible
// TODO: repair multiple packets into one long packet at the end

class FixedPoint {
public:
    FixedPoint(int32_t signed_) : mantissa(
        (uint32_t)(signed_ & 0x07ff) << 8 | signed_ & 0x8000
    ) {}
    FixedPoint(uint32_t unsigned_) : mantissa(unsigned_ << 8) {}
    FixedPoint(float float_) : FixedPoint((int32_t)(float_ * 256)) {}
private:
    int32_t mantissa;
};

class SmartPort {
public:
    SmartPort(std::ostream& stream);

    /**
     * Sends a single packet to the stream.
     * @param frame_header Header identifying the type of packet.
     * @see FrameHeader
     */
    void send(FrameHeader frame_header, uint16_t data_id, uint32_t data);

    void send(uint16_t type, uint32_t data);
private:
    /**
     * Sends one byte and
     */
    void send(uint8_t x);
    void send(uint16_t x);
    /**
     * Includes one byte in the CRC calculation.
     * @param x The raw data byte, before stuffing.
     */
    void update_crc(uint8_t x);

    std::ostream& stream;
    uint8_t crc;
};

}
