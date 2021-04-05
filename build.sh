rm -rf build && mkdir build

clang -Wall -I /usr/local/include  -framework OpenGL \
./libs/glad/src/*.c /usr/local/lib/libglfw.3.3.dylib ./src/*.c \
-o ./build/main

./build/main