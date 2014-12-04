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


######################################################
## Dependency Listings ###############################
######################################################
DEPEND_ImageBufferManager = ImageProcessing/ImageBufferManager.cpp
DEPEND_SimpleRead = ImageProcessing/SimpleRead.cpp


# Build all targets
all : mod_ImageProcessing mod_Test

## Add test cases here
test : ImageBufferManager
	./ImageBufferManager

######################################################
## mod_Test ##########################################
######################################################
mod_Test: test_ImageBufferManager test_GPIO
	
test_ImageBufferManager: ImageBufferManager
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ ImageProcessing/$@.cpp
	./$(DIR_BIN)/test_ImageBufferManager


######################################################
## mod_ImageProcessing ###############################
######################################################
mod_ImageProcessing: ImageBufferManager

ImageBufferManager: $(DEPEND_ImageBufferManager)
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DEPEND_ImageBufferManager)
	
SimpleRead: $(DEPEND_SimpleRead)
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ $(DEPEND_SimpleRead) $(LINK)

SimpleTimer: ImageProcessing/SimpleTimer.cpp
	$(CC) $(CFLAGS) -o $(DIR_BIN)/$@ ImageProcessing/$@.cpp $(LINK)



######################################################
## mod_GPIO ##########################################
######################################################
GPIO.o: GPIO/GPIO.cpp GPIO/GPIO.h
	$(CC) $(CFLAGS) -c -o $(DIR_BIN)/GPIO.o GPIO/GPIO.cpp $(LIBGPIO)

test_GPIO: GPIO.o
	$(CC) $(CFLAGS) -o $(DIR_BIN)$@ $(DIR_BIN)/GPIO.o GPIO/$@.cpp $(LINK)
	./$(DIR_BIN)/test_GPIO

######################################################
.PHONY: clean
clean:
	rm -rf bin/*
######################################################
