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
```shell
cmake ./CMakeLists.txt
make
```

# TODO
- statically link libsdl2 and liblua
- RenderLayer.apply()
- cursor
- texture rotating
- texture flipping, mirroring, etc.
- text rendering
- custom fonts
- SoundManager
- saves management
