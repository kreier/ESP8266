# RTOS SDK for ESP8266

After programming the ESP8266 with Arduino and MicroPython since 2018 I decided to develop software directly in C for this MCU. The steps in Ubuntu are stimple and straight forward. After connecting your board to USB it shows up at /dev/ttyUSB0.

## Getting started

Instructions taken from https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/ 

__Toolchain__

```
sudo apt install gcc git wget make libncurses-dev flex bison gperf python python-serial make python-pip
sudo apt install gcc git make python-pip

mkdir -p ~/esp
cd ~/esp
wget https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
tar xvzf xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
export PATH="$PATH:$HOME/esp/xtensa-lx106-elf/bin"
```

__IDF__

```
cd ~/esp
git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git
export IDF_PATH=~/esp/ESP8266_RTOS_SDK
python -m pip install --user -r $IDF_PATH/requirements.txt
```

You can check the correct set environment PATH variables with `printenv PATH` and `printenv IDF_PATH`.

__Hello world__

```
cd ~/esp
cp -r $IDF_PATH/examples/get-started/hello_world .
cd ~/esp/hello_world
make menuconfig
sudo usermod -a -G dialout $USER
sudo chmod -R 666 /dev/ttyUSB0
make flash
make monitor
```
To exit the monitor use shortcut Ctrl+].

__Wifi sniffer__
```
cp -r $IDF_PATH/examples/wifi/sniffer .
cd sniffer
make flash monitor
```
To exit the monitor use shortcut Ctrl+].

### CoreMark

```
git clone https://github.com/ochrin/coremark.git 
cd ~/esp/coremark
make flash; screen /dev/ttyUSB0 115200
```
Ctrl-a + k then y to exit screen

more to come ...
