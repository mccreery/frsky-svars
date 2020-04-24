# Smart Port (S.Port) Telemetry Library
This is a port of
[pawelsky's S.Port library](https://www.rcgroups.com/forums/showthread.php?2245978-FrSky-S-Port-telemetry-library-easy-to-use-and-configurable)
which originally was written for Arduino boards such as the Teensy or Uni. This
port is intended for use with more advanced on-board computers, so the interface
will be written using streams. The idea behind this is that you can scope your
program however you like, e.g. print to stdout and pipe into a serial port.

Until the library is written all further implementation details are undecided.

## Build
```
mkdir build && cd build && cmake .. && make
# Install
make install
```
