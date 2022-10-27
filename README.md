Copyright 2022 GrizzWorks, LLC - ALL RIGHTS RESERVED

Pico Keyer Terminal

Pico Keyer Terminal is shared in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
A Qt 5 project which provides a simple serial terminal to
work along with the Pico Keyer project.

This is a qmake project and ideally is used inside
of the Qt Creator IDE.  Open the PicoKeyer.pro file from
Qt Creator and build the project.

For Windows read the documentation
for the Qt windeployqt.exe process to create a deployable
folder for the program.

## Building from the Linux command line using qmake:

RPi/Debian-based Linux users should install the packages:

Build tools for compilers and make tools.
The basic libraries and tools for building Qt 5 qmake projects.
The development files and runtime library for serial ports in Qt5.

```
build-essential
qtbase5-dev
libqt5serialport5-dev
```

## Doing a shadow build

```
cd 
mkdir src
cd src
git clone https://github.com/guitarpicva/PicoKeyerTerminal.git
cd PicoKeyerTerminal
mkdir build
cd build
qmake ../
make -j4
```

The executable is PicoKeyerTerminal in the build folder.

##Create a home for it and assocated start script

```
cd
mkdir PicoKeyerTerminal
cd PicoKeyerTerminal
cp ~/src/PicoKeyerTerminal/build/PicoKeyerTerminal .
chmod +x PicoKeyerTerminal
echo "#!/bin/sh">PicoKeyerTerminal.sh
echo "# Start the Pico Keyer Terminal UI" >> PicoKeyerTerminal.sh
echo "export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH" >> PicoKeyerTerminal.sh
echo "./PicoKeyerTerminal &" >> PicoKeyerTerminal.sh
echo "exit 0" >> PicoKeyerTerminal.sh
chmod +x PicoKeyerTerminal.sh
```

Use the PicoKeyerTerminal.sh file to start the program from the
command line or from a shortcut in the desktop environment.
