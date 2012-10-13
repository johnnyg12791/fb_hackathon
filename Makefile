CC = g++
OBJS = main.o  
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = `pkg-config --cflags opencv` 
OPENCV = `pkg-config --libs opencv`

virtualele: $(OBJS)
	$(CC) $(LFLAGS) -o virtualele $(OBJS) $(OPENCV)
main.o: main.cpp virtualele.h uke.h boundaryMarker.h key.h body.h musicPlayer.h
	$(CC) $(CFLAGS) $(LFLAGS) main.cpp $(OPENCV)

