#include "adc.h"
#include "can.h"
#include "ecu2_sensor.h"
#include "msg_id.h"
#include "uart.h"
#include<xc.h>

void delay(unsigned short factor)
{
	unsigned short i, j;

	for (i = 0; i < factor; i++)
	{
		for (j = 500; j--; );
	}
}
void init_config()
{
    init_adc(); // configuring ADC 
    
    init_can();// configuring CAN
    
    init_digital_keypad();// configuring digital keypad
    
//    init_uart();
    
}
void main()
{
    //Call the functions
    init_config(); // configuring all required hardware software
    
    while(1)
    {
        for (unsigned int i = 30000; i--;);// blocking delay is used
        unsigned int RPM = get_rpm(); // we called rmp means we get POT(potential meter) value 
        can_transmit(RPM_MSG_ID,&RPM,2); // transmitting the data
        
        for (unsigned int i = 40000; i--;);// Bloking delay we used
        
        unsigned int indicator = process_indicator(); // clling indicator function
        can_transmit(INDICATOR_MSG_ID,&indicator,1); // using can transmitting data
        
    }
}

