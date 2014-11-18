IOLIB=-lBBBio

LED : LED.c
	gcc -o LED LED.c ${IOLIB}

Servo : Servo.c
	gcc -o Servo Servo.c ${IOLIB}

PWM : PWM.c
	gcc -o PWM PWM.c ${IOLIB}

ADC : ADC.c
	gcc -o ADC ADC.c ${IOLIB} -lm

overlay:
	

.PHONY: clean
clean :
	rm -rf *.o LED Servo PWM ADC
