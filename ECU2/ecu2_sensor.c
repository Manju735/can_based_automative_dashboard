#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"

uint16_t get_rpm()
{
    //Implement the rpm function
    unsigned int RPM = read_adc(CHANNEL4)/10.23; // we called rmp means we get POT(potential meter) value 
    return RPM = RPM * 60;// returning the RPM value calculated as 6000 as reference
}

//uint16_t get_engine_temp()
//{
//    //Implement the engine temperature function
//}

unsigned char process_indicator()
{
    //Implement the indicator function
    unsigned char key ;
    static unsigned int pressed ; //static key word used beacase it holds the update valeu every fun call
    
    key = read_digital_keypad(STATE_CHANGE); // reading the digital key
    
    if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED)) //if any switch pressed it enter to 
	{
        pressed = key;
		if(key == SWITCH1) return 0x01;
        else if(key == SWITCH2) return 0x02;
        else if(key == SWITCH3) return 0x03;
        else if(key == SWITCH4) return 0x04; 
	}
    return pressed;     // if no switch is pressed previes pressed key will return
}

