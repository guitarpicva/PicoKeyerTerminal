Copyright 2022 GrizzWorks, LLC - ALL RIGHTS RESERVED

Pico Keyer Terminal

Pico Keyer Terminal is shared in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 
A Qt 5 project which provides a simple serial terminal to
work along with the Pico Keyer project.

This is a qmake project and ideally is used inside
of the Qt Creator IDE.  Open the PicoKeyer.pro file from
Qt Creator and build the project.  See below for Linux
deployment steps.  For Windows, simply run the executable
created in Qt Creator's build folder for the project.

Building from the command line using qmake:

RPi/Debian-based Linux users should install the packages:

# build tools for compilers and make tools
build-essential
# the basic libraries and tools for building Qt 5 qmake projects
qtbase5-dev
# the development files and runtime library for serial ports
# in Qt5
libqt5serialport5-dev

# Doing a shadow build
# go to the home folder
cd 
# make src folder if it does not already exists
mkdir src
# go to the src folder
cd src
# clone the repository
git clone https://github.com/guitarpicva/PicoKeyerTerminal.git
# go to the cloned source code folder
cd PicoKeyerTerminal
# make a shadow build folder
mkdir build
# go to the build folder
cd build
# run qmake to build the Makefile
qmake ../
# make the application
make -j4

# The executable is PicoKeyerTerminal in the build folder.
# Create a home for it and assocated start script
# go to the home folder
cd
# Create a folder for the application
mkdir PicoKeyerTerminal
# go to the application folder
cd PicoKeyerTerminal
# copy the executable to the application folder
cp ~/src/PicoKeyerTerminal/build/PicoKeyer .
# Ensure that it is set to be executable
chmod +x PicoKeyer
# create a simple startup script for the application
echo "#!/bin/bash">PicoKeyerTerminal.sh
echo "# Start the Pico Keyer Terminal UI" >> PicoKeyerTerminal.sh
echo "export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH" >> PicoKeyerTerminal.sh
echo "./PicoKeyer">> PicoKeyerTerminal.sh
echo "exit 0" >> PicoKeyerTerminal.sh
# Ensure that the startup script is also executable
chmod +x PicoKeyerTerminal.sh

Use the PicoKeyerTerminal.sh file to start the program from the
command line or from a shortcut in the desktop environment.
