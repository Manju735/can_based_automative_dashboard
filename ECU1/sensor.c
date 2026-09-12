#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"

uint16_t get_speed()
{
    // Implement the speed function
    return read_adc(CHANNEL4);// reading speed using POT
    
}

/*Gear operation*/
unsigned char get_gear_pos()
{
    // Implement the gear function
    unsigned char key = 1;
    static unsigned int pressed;
    /* reading from the digital key */
    key = read_digital_keypad(STATE_CHANGE);
    
    if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED))
	{
        pressed = key;
		if(key == SWITCH1) return 0x01;
        else if(key == SWITCH2) return 0x02;
        else if(key == SWITCH3) return 0x03;
        else if(key == SWITCH4) return 0x04; 
	}
    return pressed; // returnig prevois pressed value
}