LIBNI=-lOpenNI2
LIBCV=`pkg-config opencv --cflags --libs`
CC=g++
CFLAGS=-Wall
LINK=$(LIBCV) $(LIBNI)



######################################################


SimpleRead: SimpleRead.cpp
	$(CC) $(CFLAGS) -o SimpleRead SimpleRead.cpp $(LINK)

.PHONY: clean
clean:
	rm -rf *.o SimpleRead SimpleViewer
######################################
