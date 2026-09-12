/*
 * File:   msg_handler.c
 * Author: HP
 *
 * Created on August 27, 2026, 9:10 AM
 */


#include <xc.h>
#include <string.h>
#include "msg_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"
//#include "isr.h"
unsigned int count_sec;
volatile unsigned char led_state = LED_OFF, status = e_ind_off;
//unsigned int sec_count;

void handle_speed_data(uint8_t *data, uint8_t len)
{
    
    clcd_putch((*data / 100)+'0',LINE2(0));
    clcd_putch(((*data/10)%10)+'0',LINE2(1));
    clcd_putch((*data%10)+'0',LINE2(2));
}

void handle_gear_data(uint8_t *data, uint8_t len) 
{
    //Implement the gear function
    unsigned char gear[7][3]={"GN","G1","G2","G3","G4","G5","GR"};
    static unsigned int i = 0,last_cmd = 0;
    if(i == 6 )
    {
        if(count_sec % 2 == 0 )
        RE0 = !RE0;
    }
    else
    {
        RE0 = 0;
    }
    
    if (*data != last_cmd)          
    {
        last_cmd = *data; 

        if (*data == 0x01) {
            if (i < 6) i++;
        } 
        else if (*data == 0x02) 
        {
            if (i > 0) i--;
        }                
        
    }
    if(*data != 0x03)
    clcd_print(gear[i], LINE2(4)); 
    else 
    clcd_print("_C", LINE2(4));
}
/*RPM data printing in clcd disp*/
void handle_rpm_data(uint8_t *data, uint8_t len) 
{
    //Implement the rpm function
    uint16_t rpm = data[0] | ((uint16_t )data[1] << 8) ;
    
    clcd_putch((rpm/1000)+'0',LINE2(7));
    clcd_putch(((rpm/100)%10)+'0',LINE2(8));
    clcd_putch(((rpm/10)%10)+'0',LINE2(9));
    clcd_putch((rpm%10)+'0',LINE2(10));
}

/*The indicator it will show based on switch we presed*/
void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    //Implement the indicator function
    static unsigned int cmd_data = 0;
    if(*data != cmd_data)
    {
        cmd_data = *data;
        switch(*data)
        {
            case 0x01:
                clcd_print("   ",LINE2(12));
                clcd_print("<--",LINE2(12));
            break;
            case 0x02:
                clcd_print("   ",LINE2(12));
                clcd_print("<-->",LINE2(12));
            break;
            case 0x03:
                clcd_print("   ",LINE2(12));
                clcd_print("-->",LINE2(12));
            break;
            case 0x04:
                clcd_print("   ",LINE2(12));
            break;
        }
    }
    if(cmd_data == 1)
    {
        if(count_sec % 2 == 0)
        {
            LEFT_IND_ON();
            RE0 = 0;
        }
        else
        {
            LEFT_IND_OFF();  RE0 = 1;
        } 
    }
    else if(cmd_data == 2)
    {
        if(count_sec % 2 == 0)
        {
            RIGHT_IND_ON();
            LEFT_IND_ON();
            RE0 = 0;
        }
        else
        {
            RIGHT_IND_OFF();
            LEFT_IND_OFF();  RE0 = 1;
        }
    }
    else if(cmd_data == 3)
    {
        if(count_sec % 2 == 0)
        {
            RIGHT_IND_ON();
            RE0 = 0;
        }
        else
        {
            RIGHT_IND_OFF();  RE0 = 1;
        }
    }
    else
    {
        RE0 = 0;
        LEFT_IND_OFF();
        RIGHT_IND_OFF();
    }        
}

void process_canbus_data() 
{   
    //process the CAN bus data
    uint8_t data[8],len = 0;
    uint16_t msg_id = 0;
    /*calling can receive function and coolecting message identifier(MI), data, length*/
    can_receive(&msg_id,data,&len);
    /*clcd LINE1 displaying data identifier like*/
    clcd_print("SPD",LINE1(0));// speed
    clcd_print("GR",LINE1(4));// gear
    clcd_print("RPM",LINE1(7));//RPM
    clcd_print("IND",LINE1(12));//indicator
    
    /*Based on msg id switch case will execute*/
    switch(msg_id)
    {
        case SPEED_MSG_ID:
            handle_speed_data(data,len);
            break;
        case GEAR_MSG_ID:
            handle_gear_data(data,len);
            break;
        case RPM_MSG_ID:
            handle_rpm_data(data,len);
            break;
        case INDICATOR_MSG_ID :
            handle_indicator_data(data,len);
            break;
    }
}
