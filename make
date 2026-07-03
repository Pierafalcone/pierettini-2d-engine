clang -o main main.c game.c draw.c src/glad.c -lSDL2 -Iinclude/
cppcheck main.c game.c draw.c
./main