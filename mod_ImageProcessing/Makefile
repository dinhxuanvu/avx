NILIB=-lOpenNI2
CC=g++
CFLAGS=-Wall



######################################################


SimpleRead: SimpleRead.cpp
	$(CC) $(CFLAGS) -o SimpleRead SimpleRead.cpp $(NILIB)

Viewer: Viewer.cpp Viewer.h
	$(CC) $(CFLAGS) -c Viewer.cpp $(NILIB)

SimpleViewer: SimpleViewer.cpp Viewer
	$(CC) $(CFLAGS) -o SimpleViewer SimpleViewer.cpp Viewer.o $(NILIB)


.PHONY: clean
clean:
	rm -rf *.o SimpleRead SimpleViewer
