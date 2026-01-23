FILES=./build/main.o ./build/file-win.o ./build/contents-win.o  \
./build/help-win.o ./build/menubar-win.o 			            \
./build/debug-win.o ./build/win-manager.o                       \
./build/filter-form.o
INCLUDES=
CFLAGS=-g

all: ${FILES}
	gcc -o main ${CFLAGS} ${INCLUDES} ${FILES} -lncurses

clean:
	rm -f ${FILES} main

./build/main.o: src/main.c
	gcc ${CFLAGS} -o build/main.o -c src/main.c

./build/file-win.o: src/file-win.c
	gcc ${CFLAGS} -o build/file-win.o -c src/file-win.c

./build/contents-win.o: src/contents-win.c
	gcc ${CFLAGS} -o build/contents-win.o -c src/contents-win.c

./build/help-win.o: src/help-win.c
	gcc ${CFLAGS} -o build/help-win.o -c src/help-win.c

./build/filter-form.o: src/filter-form.c
	gcc ${CFLAGS} -o build/filter-form.o -c src/filter-form.c

./build/menubar-win.o: src/menubar-win.c
	gcc ${CFLAGS} -o build/menubar-win.o -c src/menubar-win.c

./build/debug-win.o: src/debug-win.c
	gcc ${CFLAGS} -o build/debug-win.o -c src/debug-win.c

./build/win-manager.o: src/win-manager.c
	gcc ${CFLAGS} -o build/win-manager.o -c src/win-manager.c
