rm -rf build

mkdir build

clang -g -Wall \
-I /usr/local/include -I ./libs -framework OpenGL \
/usr/local/lib/libglfw.3.3.dylib ./libs/**/*.c ./src/*.c \
-o ./build/main