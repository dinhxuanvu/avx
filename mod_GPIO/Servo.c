
#include <stdio.h>
#include <stdlib.h>
#include <BBBio_lib/BBBiolib.h>
/* ----------------------------------------------------------- */
int main(void)
{
	int i;
  /* Initialize GPIO library */
	iolib_init();
	/* Sermotor 1 , connect to ePWM0A */
	float SM_1_duty ;

	for(i = 0 ; i <= 180 ; i += 10)
	{
    M_1_duty  = 4.0f + i * 0.04444f ;
	  printf("Angle : %d , duty : %f\n" ,i ,SM_1_duty);
	  BBBIO_PWMSS_Setting(BBBIO_PWMSS0 , 50.0f ,SM_1_duty , SM_1_duty);
		BBBIO_ehrPWM_Enable(BBBIO_PWMSS0);
	  sleep(2);
	}
  
  BBBIO_ehrPWM_Disable(BBBIO_PWMSS0);

  /* Deintialize GPIO library */
	iolib_free();
	return(0);
}


