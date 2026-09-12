/*
 * File:   isr.c
 * Author: HP
 *
 * Created on August 27, 2026, 9:14 AM
 */


#include <xc.h>

extern unsigned int count_sec;
void __interrupt() isr(void)
{
    static unsigned int count = 0;
    if(TMR0IF == 1)
    {
        if(count++ == 160)
        {
            count = 0;
            if(count_sec++ == 60)
                count_sec = 0;
        }
        TMR0IF = 0;
    }
}