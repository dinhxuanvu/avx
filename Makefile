LIBNI=-lOpenNI2
LIBCV=`pkg-config opencv --cflags --libs`
LIBGPIO=-lBBBio
CC=g++
CFLAGS=-Wall
LINK=$(LIBCV) $(LIBNI) $(LIBGPIO)
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

test_GPIO: GPIO
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ GPIO.o mod_Test/$@.cpp $(LINK)
	

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
GPIO: mod_GPIO/GPIO.cpp mod_GPIO/GPIO.h
	$(CC) $(CFLAGS) -c mod_GPIO/$@.cpp $(LIBGPIO)

######################################################
.PHONY: clean
clean:
	rm -rf bin/*
######################################################
