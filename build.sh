rm -rf build && mkdir build

clang -Wall -I /usr/local/include -framework OpenGL \
./src/*.c /usr/local/lib/libglfw.3.3.dylib \
-o ./build/main

./build/main