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
    /**
     * Corresponds to variable 0. Variable IDs 0-255 can be ORed with this to
     * calculate the data ID for the corresponding variable.
     * @see var_max
     */
    var_0 = 0x5100,
    var_max = 0x51ff
};

// TODO: in 32 bits, stuff as much data into the packet as possible
// TODO: repair multiple packets into one long packet at the end

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
