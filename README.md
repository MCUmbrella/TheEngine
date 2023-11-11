# Requirements
- gcc (with C++ 20 support)
- make
- cmake 3.18
- liblua5.4-dev
- libsdl2-dev
- libsdl2-gfx-dev
- libsdl2-image-dev
- libsdl2-mixer-dev
- libsdl2-ttf-dev

# Build
The following instructions is recommended, but not the only way to build the program.
## Set up environment
Recommended software:
- CLion
- Visual Studio Code
### Operating system
Kubuntu 21.10
### /etc/apt/sources.list
```
deb http://old-releases.ubuntu.com/ubuntu impish main restricted
deb http://old-releases.ubuntu.com/ubuntu impish universe
deb http://old-releases.ubuntu.com/ubuntu impish multiverse
deb http://old-releases.ubuntu.com/ubuntu impish-security main restricted
deb http://old-releases.ubuntu.com/ubuntu impish-security universe
deb http://old-releases.ubuntu.com/ubuntu impish-security multiverse
```
### Install dependencies
```shell
sudo apt update
sudo apt install g++ make cmake liblua5.4-dev libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```
## Compile
```shell
cmake ./CMakeLists.txt
make
```
## Clean
```shell
rm -rf ./CMakeFiles
rm -f ./cmake_install.cmake ./CMakeCache.txt ./Makefile ./TheEngine
```

# TODO
- statically link libsdl2 and liblua
- text rendering
- custom fonts
- saves management
