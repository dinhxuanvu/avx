# Library include for OpenNI Primesense driver
LIBNI=-lOpenNI2
# Library includes necessary for OpenCV compilation
LIBCV=`pkg-config opencv --cflags --libs`
# Library includes for BeagleBone GPIO library
LIBGPIO=-lBBBio
# Default compiler
CC=g++
# Compile flags
CFLAGS=-Wall
# Link for all libraries needed
LINK=$(LIBCV) $(LIBNI) $(LIBGPIO) -lboost_thread-mt
# Directory to place binary executables
DIR_BIN=bin



# Build all targets
all : ImageProcessor

## Add test cases here
test : BufferManager.o
	./$(DIR_BIN)/test_BufferManager


######################################################
## mod_Test ##########################################
######################################################


######################################################
## mod_BufferManager ###############################
######################################################
BufferManager.o: BufferManager/BufferManager.cpp BufferManager/BufferManager.h
	$(CC) $(CFLAGS) -c -o $(DIR_BIN)/$@ BufferManager/BufferManager.cpp
	
test_BufferManager: BufferManager.o BufferManager/test_BufferManager.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DIR_BIN)/BufferManager.o BufferManager/$@.cpp $(LIBNI) -lboost_thread -lboost_system 

test_BufferManager_threads:test_BufferManager
	./$(DIR_BIN)/test_BufferManager
	
	
######################################################
## mod_Camera ########################################
######################################################
Camera: SimpleRead

SimpleRead: Camera/SimpleRead.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleRead.cpp $(LIBCV) $(LIBNI)

SimpleTimer: Camera/SimpleTimer.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleTimer.cpp $(LIBCV) $(LIBNI)

######################################################
## mod_ImageProcessor ################################
######################################################
ImageProcessor.o: ImageProcessor/ImageProcessor.cpp ImageProcessor/ImageProcessor.h
	$(CC) $(CFLAGS) -c -o $(DIR_BIN)/ImageProcessor.o ImageProcessor/ImageProcessor.cpp

test_ImageProcessor: ImageProcessor.o
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DIR_BIN)/ImageProcessor.o ImageProcessor/$@.cpp $(LIBCV)

######################################################
## mod_GPIO ##########################################
######################################################
GPIO.o: GPIO/GPIO.cpp GPIO/GPIO.h
	$(CC) $(CFLAGS) -c -o $(DIR_BIN)/GPIO.o GPIO/GPIO.cpp $(LIBGPIO)

test_GPIO: GPIO.o
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DIR_BIN)/GPIO.o GPIO/$@.cpp $(LINK)

######################################################
.PHONY: clean
clean:
	rm -rf $(DIR_BIN)/*
######################################################
