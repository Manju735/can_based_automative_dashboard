#include "adc.h"
#include "can.h"
#include "ecu1_sensor.h"
#include "msg_id.h"
#include "uart.h"

#include<xc.h>

/*Configuring required pheriperals*/
void init_config()
{
    init_adc();
    
    init_can();
    
    init_digital_keypad();
    
//    init_uart();
    
}
void main()
{
    //Call the functions
    init_config();
    
    while(1)
    {
        /*Calling speed function mens using POT we get is*/
        unsigned int speed = get_speed()/10.23;
        can_transmit(SPEED_MSG_ID,&speed,1);// speed value will be transmitted with message identifier(MI)
        for (unsigned int j = 10000; j--; );
        
        /*Gear function calling data transmitting with MI*/
        unsigned int gear = get_gear_pos();
        can_transmit(GEAR_MSG_ID,&gear,1);
        for (unsigned int j = 20000; j--; );
    }
}

