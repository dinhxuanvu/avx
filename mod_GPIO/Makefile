IOLIB=-lBBBio
CC=g++

all : GPIO run

GPIO : GPIO.cpp GPIO.h
	$(CC) -o GPIO GPIO.cpp ${IOLIB}

run : GPIO
	sudo ./GPIO 1 100

ADC : ADC.c
	gcc -o ADC ADC.c ${IOLIB} -lm

overlay:
	

.PHONY: clean
clean :
	rm -rf *.o LED Servo PWM ADC
