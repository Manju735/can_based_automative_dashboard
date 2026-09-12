#include"uart.h"
void init_uart()
{
    TRISC6 = 0;//TX TRANSMITTING
    TRISC7 = 1;//RX RECEVING 
    TXEN = 0;// TRANSMITTING DATA
    TX9 = 0;
    SYNC = 0;
    BRGH = 1;
    TXEN = 1;
    
    SPEN = 1;// RECEING DATA
    RX = 0;
    CREN = 1;
    
    // BOUD RATE DETECTION
    BRG16 = 0;
    ABDOVF = 1;
    ABDEN = 0;
    SPBRG = 129;
}

void uart_char_transimit(unsigned char ch)
{
    while(!TXIF);
    
    TXREG = ch;
}

void uart_str_transmit(unsigned char *str)
{
    while(*str)
    {
        uart_char_transimit(*str++);
    }
}

unsigned char uart_receive(void)
{
    while(!RCIF);
    
    return RCREG;
}