## EMG Project

### Clone the repository

**git clone https://github.com/IlievIliya92/EMG.git**

### Prerequisites

Run the following command to install required packages:

**sudo apt install cmake gcc-avr avr-libc avrdude python-tk**

Run the following commands to make the scripts executable:

**chmod +x emg_configure emg emg_check_requirements**

### 1. Check for required python packages
    ./emg_check_requirements

### 2. Configure the project.
    source emg_configure

### 3. Connect the board using this schematic

Emg-Arduino Schematic
![alt text](https://github.com/IlievIliya92/EMG/blob/master/source/emg_arduino/schematic/EMG_Arduino.png
 "Emg Arduino schemtaic")

### 4. Build the project
    emg_build

### 5. Upload to the target
    emg_upload [name_of_the_serial_device]

The "L" debug led on the board should start blinking. Indicating
that the sampliing of the channels is working properly.

### Usage
Run the emg application:

    ./emg -s [serial_port]
