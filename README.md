# au_teensycode

This repository is to be used for Teensy 3.2 firmware development for Auri.

## Getting Started

To start coding for a Teensy used in a specific board, clone the develop branch and 
create a new branch with the name of the board. For consistency, use the [Arduino]("arduino.cc")
framework. Open the project using [PlatformIO]("platformio.org").

### PlatformIO Libraries

Libraries in PlatformIO must be placed in the project's lib/ folder with the following structure:
lib/{library name}/src/codefile.(h, c, hpp, cpp).

## LibUAVCAN

To install libuavcan, clone the branch and run `git submodule update --init --remote`.
Make sure `lib/libuavcan` is on branch `teensy-driver` and updated to have the latest
implementation of libuavcan.

Test applications for UAVCAN implementation consist of a `publisher.cpp`
and a `subscriber.cpp`, one for publishing LogMessages and the other one subscribed to
them dumping stuff on Serial port, each on their own branches.

Refer to [uavcan.org]("uavcan.org") for details on the UAVCAN protocol.

### Custom UAVCAN Data Types

For creating custom datatypes for UAVCAN, DSDL files need to be created. These files
must be placed in the lib/arvp/arvp/ folder. The files must be named like so:
{data-type-ID}.{Dataname}.uavcan. Note that the first letter of the data name must
be capitalized.

After creating the DSDL files, run the following command in a terminal at the project
root to generate the C code: 
`lib/libuavcan/libuavcan/dsdl_compiler/libuavcan_dsdlc ./lib/arvp/arvp -Olib/arvp/src`

In the project source files, add include statements for the .hpp files for each
data type required by the Teensy program. ex: `#include <arvp/Test.hpp>`

## Build

In PlatformIO just hit the build button and magic happens....
