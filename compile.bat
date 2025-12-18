mkdir build
cmake --preset=debug -DCMAKE_TOOLCHAIN_FILE="C:/Users/rodie/vcpkg/scripts/buildsystems/vcpkg.cmake"
mingw32-make -C ./build
mv ./build/compile_commands.json ./compile_commands.json
PAUSE
