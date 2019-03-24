# au_teensycode

This repository is to be used for Teensy 3.2 firmware development for Auri.

## Getting Started

To start coding for a Teensy used on a specific board, clone the develop branch and copy the example or the uavcan_example folder, depending on whether UAVCAN is required for your application. Rename the folder to the name of the board. For consistency, use the [Arduino](http://arduino.cc) framework. Open the project using [PlatformIO](http://platformio.org). Rename the src/main.cpp file to something more relevant and write your main program there.

### Build

The code for a particular board is stored in `src/{environment}`. Add new environments in the `platformio.ini` file. Copy the configurations of the example programs and rename the `src_filter` parameter to the name of your source folder. To only build / upload one specific environment use the following command:

`pio run -t upload -e {environment}`   ([more info](http://docs.platformio.org/en/latest/userguide/cmd_run.html#cmdoption-platformio-run-e))

### PlatformIO Libraries

Libraries in PlatformIO must be placed in the project's lib/ folder with the following structure:
lib/{library name}/src/codefile.(h, c, hpp, cpp).

## Libuavcan

### System requirements
In order to compile libuavcan, PlatformIO needs to run a linux environment such as Ubuntu, Mint, Debian, etc. Native or virtual machine installations are equally acceptable.

### Examples

The teensy_example UAVCAN implementation program consists of header files called `publisher.cpp`, `subscriber.cpp`, and `parameters.hpp`  for demonstrating publishing and subscribing to a UAVCAN message called LogMessages and dumping stuff on the serial port.

Refer to [uavcan.org](http://uavcan.org) for details on the UAVCAN protocol.

### Custom UAVCAN Data Types

For creating custom data types for UAVCAN, DSDL files need to be created. These files must be placed in the lib/arvp/arvp/ folder. The files must be named like so:

`{data-type-ID}.{Datatype}.uavcan`

Note that the first letter of the data type must be capitalized. The data type ID should be between 20000 and 21000 per the UAVCAN "vendor-specific message type" convention. Refer to the [DSDL documentation](http://uavcan.org/Specification/3._Data_structure_description_language/) for details on the data structure.

Place the DSDL files in the folder lib/arvp_msgs. The DSDL compiler scripts bundled in libuavcan automatically generate the .hpp files
from the DSDL files. In the project source code, add include statements for the .hpp files for each
data type required by the Teensy program. ex: `#include <arvp_msgs/Test.hpp>`

### Notes on Merging Old UAVCAN Project Branches

When integrating existing code into this new framework:
- Delete the old 'teensy_uavcan.hpp' file in the src/ folder in favour of the one in lib/libuavcan
- Ensure that .hpp files in the src/ folder are included using "" instead of <>
- Change all inclusions of <UAVCAN.hpp> to <uavcan/uavcan.hpp> in publisher.hpp, subscriber.hpp, and parameter.hpp
- In the Setup() function, change `&getSystemClock()` and 
    `&getCanDriver()` to `&initSystemClock()` and 
    `&initCanDriver()`
- In the Setup() function, add the following line:  `configureCanAcceptanceFilters(*node);`. Refer to uavcan_example program.