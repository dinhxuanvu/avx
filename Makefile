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
LINK=$(LIBCV) $(LIBNI) $(LIBGPIO)
# Directory to place binary executables
DIR_BIN=bin



# Build all targets
all : ImageProcessing

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
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DIR_BIN)/BufferManager.o BufferManager/$@.cpp $(LIBNI)

	
######################################################
## mod_Camera ########################################
######################################################
Camera: SimpleRead

SimpleRead: Camera/SimpleRead.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleRead.cpp $(LIBCV) $(LIBNI)

SimpleTimer: Camera/SimpleTimer.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ Camera/SimpleTimer.cpp $(LIBCV) $(LIBNI)



######################################################
## mod_GPIO ##########################################
######################################################
GPIO.o: GPIO/GPIO.cpp GPIO/GPIO.h
	$(CC) $(CFLAGS) -c -o $(DIR_BIN)/GPIO.o GPIO/GPIO.cpp $(LIBGPIO)

test_GPIO: GPIO.o
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DIR_BIN)/GPIO.o GPIO/$@.cpp $(LINK)
	./$(DIR_BIN)/test_GPIO

######################################################
.PHONY: clean
clean:
	rm -rf $(DIR_BIN)/*
######################################################
