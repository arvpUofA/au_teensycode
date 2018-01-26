[![Build Status](https://travis-ci.org/tum-phoenix/drive_teensy_tester.svg?branch=master)](https://travis-ci.org/tum-phoenix/drive_teensy_tester)
# au_teensycode
This repository is for Teensy 3.2 firmware development for Auri.

Test applications for UAVCAN implementation consist of a `publisher.cpp`
and a `subscriber.cpp`, one for publishing LogMessages and the other one subscribed to
them dumping stuff on Serial port, each on their own branches.

## Install

Clone and run `git submodule update --init --remote`. Make sure `lib/libuavcan`
is on branch `teensy-driver` and updated to have the latest implementation of libuavcan.

## Build

In PlatformIO just hit the build button and magic happens....
