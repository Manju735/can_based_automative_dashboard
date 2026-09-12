#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /*Setting digital keypad input*/
	TRISC = TRISC | INPUT_PINS;
}

/*Reading Digital Keypad*/
unsigned char read_digital_keypad(unsigned char detection_type)
{
	static unsigned char once = 1;
    /*Edge Triggering or level Triggering logic Based on detection type it will Execute*/
	if (detection_type == STATE_CHANGE)
	{
		if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED) && once)
		{
			once = 0;

			return (KEY_PORT & INPUT_PINS);
		}
		else if ((KEY_PORT & INPUT_PINS) == ALL_RELEASED)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL)
	{
		return (KEY_PORT & INPUT_PINS);
	}

	return 0xFF;// if no key is pressed means all key are realesd 
}