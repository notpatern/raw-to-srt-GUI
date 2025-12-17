mkdir build

cmake --preset=debug
mingw32-make -C ./build

mv ./build/compile_commands.json ./compile_commands.json

PAUSE
