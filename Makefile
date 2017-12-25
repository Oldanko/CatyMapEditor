CC=g++
CFLAGS= -I include -I /usr/include/libdrm  -std=c++11
LIBS=-lglfw -lGL -lGLEW -lSOIL

output: main.o level.o saver.o controls.o platform.o
	$(CC) -o bin/output build/main.o build/level.o build/saver.o build/controls.o build/platform.o $(LIBS) $(CFLAGS)

main.o: src/main.cpp
	$(CC) -c src/main.cpp -o build/main.o 	 $(LIBS) $(CFLAGS)

level.o: src/level.cpp
	$(CC) -c src/level.cpp -o build/level.o  $(LIBS) $(CFLAGS)

saver.o: src/saver.cpp
	$(CC) -c src/saver.cpp -o build/saver.o  $(LIBS) $(CFLAGS)

controls.o: src/controls.cpp
	$(CC) -c src/controls.cpp -o build/controls.o  $(LIBS) $(CFLAGS)

platform.o: src/platform.cpp
	$(CC) -c src/platform.cpp -o build/platform.o  $(LIBS) $(CFLAGS)


clean:
	rm -rf build/*.o bin/output
