# Library include for OpenNI Primesense driver
LIBNI=-lOpenNI2
# Library includes necessary for OpenCV compilation
LIBCV=`pkg-config opencv --cflags --libs`
# Library includes for BeagleBone GPIO library
LIBGPIO=-lBBBio
# Default compiler
CC=g++

DEBUG=1
# Compile flags
CFLAGS=-Wall -DDEBUG=$(DEBUG)
# Link for all libraries needed
LINK=$(LIBCV) $(LIBNI) $(LIBGPIO)
# Directory to place binary executables
DIR_BIN=bin

# Build all targets
all: bin/avx
	sudo ./bin/avx

compile: bin/avx

######################################################
## Main program ######################################
######################################################
bin/avx: Main.cpp bin/ImageProcessor.o bin/BufferManager.o bin/Camera.o
	$(CC) $(CFLAGS) -o $@  bin/ImageProcessor.o bin/BufferManager.o bin/Camera.o Main.cpp $(LIBNI) -lboost_thread -lboost_system $(LIBCV)
######################################################
## mod_Test ##########################################
######################################################
## Add custom make targets for testing here

######################################################
## mod_BufferManager #################################
######################################################
bin/BufferManager.o: BufferManager/BufferManager.cpp BufferManager/BufferManager.h
	$(CC) $(CFLAGS) -c -o $@ BufferManager/BufferManager.cpp `pkg-config opencv --cflags` 
	
test_BufferManager: bin/BufferManager.o BufferManager/test_BufferManager.cpp
	$(CC) $(CFLAGS) -o bin/$@ bin/BufferManager.o BufferManager/$@.cpp $(LIBCV) $(LIBNI) -lboost_thread -lboost_system 
	
######################################################
## mod_Camera ########################################
######################################################
bin/Camera.o: Camera/Camera.cpp Camera/Camera.h
	$(CC) $(CFLAGS) -c -o $@ Camera/Camera.cpp $(LIBNI)

#SimpleRead: Camera/SimpleRead.cpp
#	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleRead.cpp $(LIBCV) $(LIBNI)

#SimpleTimer: Camera/SimpleTimer.cpp
#	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleTimer.cpp $(LIBCV) $(LIBNI)

######################################################
## mod_ImageProcessor ################################
######################################################
bin/ImageProcessor.o: ImageProcessor/ImageProcessor.cpp ImageProcessor/ImageProcessor.h
	$(CC) $(CFLAGS) -c -o $@ ImageProcessor/ImageProcessor.cpp `pkg-config opencv --cflags` 

test_ImageProcessor: bin/ImageProcessor.o
	$(CC) $(CFLAGS) -o bin/$@ bin/ImageProcessor.o ImageProcessor/$@.cpp $(LIBCV)

######################################################
## mod_PathPlanner ################################
######################################################
bin/PathPlanner.o: PathPlanner/PathPlanner.cpp PathPlanner/PathPlanner.h
	$(CC) $(CFLAGS) -c -o $@ PathPlanner/PathPlanner.cpp

test_PathPlanner: bin/PathPlanner.o
	$(CC) $(CFLAGS) -o bin/$@ bin/PathPlanner.o PathPlanner/$@.cpp

######################################################
## mod_Control ################################
######################################################
bin/Control.o: Control/Control.cpp Control/Control.h
	$(CC) $(CFLAGS) -c -o $@ Control/Control.cpp

test_Control: bin/Control.o
	$(CC) $(CFLAGS) -o bin/$@ bin/Control.o Control/$@.cpp

######################################################
## mod_GPIO ##########################################
######################################################
bin/GPIO.o: GPIO/GPIO.cpp GPIO/GPIO.h
	$(CC) $(CFLAGS) -c -o $@ GPIO/GPIO.cpp

test_GPIO: bin/GPIO.o
	$(CC) $(CFLAGS) -o bin/$@ bin/GPIO.o GPIO/$@.cpp $(LIBGPIO)

######################################################
.PHONY: clean
clean:
	rm -rf bin/*
######################################################
