# S.Port (Smart Port) Telemetry Library
This is a minimal library for sending a superset of FrSky's S.Port passthrough
telemetry using C++ streams. Most existing libraries are written for Arduino
boards such as the Teensy or Uno. This library builds to a standard relocatable
(.so) library on Linux systems.

## Build

### Requirements
- ROS (for Catkin which is used by the serial library)
- CMake

```bash
# Source ROS environment
. /opt/ros/melodic/setup.sh

# Generate libsport.so and sport-cli
mkdir -p build
cd build
cmake ..
make
# Install libsport.so and libserial.so
make install
```

## Library

See header [`<frsky/sport.hpp>`](include/frsky/sport.hpp) for the public API.
- `sport::FixedPoint` allows sending fixed point numbers over the S.Port
- `sport::configure_serial` configures an unopened serial object for S.Port
- `sport::putvar` sends one or more telemetry packets over the given serial
  port, which are compatible with the Lua scripts in [the root
  project](https://github.com/mccreery/comp3200-project)

## CLI
There is a basic command-line interface `sport_cli`:

```shell
$ sudo ./sport_cli /dev/ttyS0
S.Port CLI v0.1
>
Expected channel (0-63)
Command format: [CHANNEL (0-63)] [TYPE (int, fixed, float, string)] [VALUE]
>
```

**The following information is a work in progress.**

## Terminology
Term        | Meaning
------------|--------
FrSky D     | Older protocol on non-X series FrSky receivers
S.Port      | Smart Port, a telemetry protocol designed for FrSky receivers
Passthrough | An extension to S.Port with a "private use" range of IDs
OpenTX      | Transmitter firmware with support for S.Port decoding in Lua

## (Known) Protocol
One packet:
Byte index | Data
-----------|-----
0          | Physical sensor ID (ignored, assuming 0 is fine)
1          | Frame header (`0x10` or `0x32` to always passthrough)
2-3        | Data ID (passthrough IDs range `0x5000` to `0x52ff`)
4-7        | Value (format depends on data ID)
8          | CRC of entire packet (untested)

- Multi-byte values are big-endian (untested)
- CRC algorithm is as follows:
  - CRC starts at 0
  - For each byte, add to the CRC to produce a 9-bit result
  - Add to the result itself shifted right by 8 bits
  - Take the least significant byte of the result as the next CRC
  - The CRC is inverted (`~crc`) before including in the packet

## Sources of Information
Information surrounding the S.Port specification is limited, since the protocol
is proprietary property of FrSky.

- [pawelsky's S.Port
  library](https://www.rcgroups.com/forums/showthread.php?2245978-FrSky-S-Port-telemetry-library-easy-to-use-and-configurable)
  describes an older version of the protocol, using 8-bit data IDs rather than
  16-bit
- [ArduPilot
  documentation](https://ardupilot.org/copter/docs/common-frsky-passthrough.html)
  includes a spreadsheet of 16-bit data IDs in the new protocol
- [FrSkySP
  documentation](https://www.ordinoscope.net/static/arduino-frskysp/docs/html/)
  explains the packet structure (especially the "Sensor behaviour" section)
- [D protocol
  manual](https://www.frsky-rc.com/wp-content/uploads/2017/07/Manual/protocol_sensor_hub.pdf)
  (old protocol, see above)
- [OpenTX
  source](https://github.com/opentx/opentx/blob/2.3/radio/src/telemetry/frsky.h)
  handles both D and S.Port telemetry
- [OpenTX Lua
  reference](https://opentx.gitbooks.io/opentx-2-2-lua-reference-guide/content/general/sportTelemetryPop.html)
  gives hints of packet structure
- [PX4
  source](https://github.com/PX4/Firmware/blob/master/src/drivers/telemetry/frsky_telemetry/frsky_telemetry.cpp)
  explaining serial port parity, stop bits etc.
