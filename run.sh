rm -rf build && mkdir build

clang -Wall -I /usr/local/include  -framework OpenGL \
/usr/local/lib/libglfw.3.3.dylib ./libs/glad/src/*.c ./src/*.c \
-o ./build/main

./build/main