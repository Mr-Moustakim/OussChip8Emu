INCLUDE= -I ./include
FLAGS= -std=c99 -Wall -g
OBJECTS= ./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o ./build/chip8Core.o ./build/chip8display.o ./build/chip8opcode.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDE} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8memory.o: src/chip8memory.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8memory.c -c -o ./build/chip8memory.o
./build/chip8stack.o: src/chip8stack.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8stack.c -c -o ./build/chip8stack.o
./build/chip8keyboard.o: src/chip8keyboard.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o
./build/chip8Core.o: src/chip8Core.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8Core.c -c -o ./build/chip8Core.o
./build/chip8display.o: src/chip8display.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8display.c -c -o ./build/chip8display.o
./build/chip8opcode.o: src/chip8opcode.c
	gcc ${FLAGS} ${INCLUDE} ./src/chip8opcode.c -c -o ./build/chip8opcode.o
	
clean:
	del build\*
