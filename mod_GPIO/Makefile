IOLIB=-lBBBio

all : GPIO run

GPIO : GPIO.c GPIO.h
	gcc -o GPIO GPIO.c ${IOLIB}

run : GPIO
	sudo ./GPIO 1 100

ADC : ADC.c
	gcc -o ADC ADC.c ${IOLIB} -lm

overlay:
	

.PHONY: clean
clean :
	rm -rf *.o LED Servo PWM ADC
