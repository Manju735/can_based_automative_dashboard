/*
 * File:   main.c
 * Author: HP
 *
 * Created on August 27, 2026, 9:10 AM
 */


#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "msg_handler.h"
//#include "isr.h"
#include "timer0.h"

void init_leds() 
{
    TRISB = 0x00; 
    
}
void init_buzzer()
{
    TRISE0 = 0;
    RE0 = 0;
}
static void init_config(void) {
    // Initialize CLCD and CANBUS
    init_clcd();
    
    init_leds();
    init_buzzer();
    init_can();

    // Enable Interrupts
    PEIE = 1;
    GIE = 1;
    init_timer0();
}

void main(void) {
    // Initialize peripherals
    init_config();

    /* ECU1 main loop */
    while (1) {
        // Read CAN Bus data and handle it
        process_canbus_data();
    }

    return;
}

