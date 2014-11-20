LIBNI=-lOpenNI2
LIBCV=`pkg-config opencv --cflags --libs`
CC=g++
CFLAGS=-Wall
LINK=$(LIBCV) $(LIBNI)
DIR_BIN=bin/


all:mod_ImageProcessing mod_Test

## Add test cases here
test:all
	./ImageBufferManager

######################################################
## mod_Test ##########################################
######################################################
mod_Test: test_ImageBufferManager
	./ImageBufferManager
	
test_ImageBufferManager: ImageBufferManager
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ mod_Test/$@.cpp
	

######################################################
## mod_ImageProcessing ###############################
######################################################
mod_ImageProcessing: ImageBufferManager

ImageBufferManager: mod_ImageProcessing/ImageBufferManager.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ mod_ImageProcessing/$@.cpp
	
SimpleRead: mod_ImageProcessing/SimpleRead.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ mod_ImageProcessing/$@.cpp $(LINK)


######################################################
## mod_GPIO ##########################################
######################################################
#mod_GPIO:



######################################################
.PHONY: clean
clean:
	rm -rf *.o SimpleRead SimpleViewer
######################################################
