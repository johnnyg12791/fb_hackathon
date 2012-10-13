CC = g++
OBJS = main.o 
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = `pkg-config --cflags opencv`
OPENCV = `pkg-config --libs opencv`

virtualele: $(OBJS)
	$(CC) $(LFLAGS) -o virtualele $(OBJS) $(OPENCV)
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(OPENCV)

