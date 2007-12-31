SRC = ./src/
BIN = ./bin/
FLAGS = -Wall `pkg-config --cflags --libs gtk+-2.0` `vlc-config --cflags --libs vlc` -I/usr/src/vlc-trunk/include -lvlc

all: ${BIN}main

${BIN}main: ${SRC}main.c
	gcc ${SRC}main.c -o ${BIN}main ${FLAGS}

run: ${BIN}main
	${BIN}main

