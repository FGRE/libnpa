#!/bin/bash
export PATH=$MXE/usr/bin
i686-pc-mingw32-g++ -std=gnu++11 -mwindows -shared src/*.cpp -I include/ -o libnpa.dll -lboost_locale-mt -lboost_system-mt
