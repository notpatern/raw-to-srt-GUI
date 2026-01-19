mkdir build
cmake -S . ./build
make -C ./build
mv ./build/compile_commands.json ./compile_commands.json
