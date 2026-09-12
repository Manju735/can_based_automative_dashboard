#include"uart.h"
void init_uart()
{
    TRISC6 = 0;//TX TRANSMITTING
    TRISC7 = 1;//RX RECEVING 
    TXEN = 0;// TRANSMITTING DATA off initially
    TX9 = 0;// transmission 8/9 bit selecting
    SYNC = 0;
    BRGH = 1;// boude rate selecting high speed 
    TXEN = 1; //transmission enabling
    
    SPEN = 1;// RECEING DATA port will on
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
    while(!TXIF); //waiting previos data not transmitted
    
    TXREG = ch; // byte by byte data transmitting
}

/*This Function collecting full string & transmitting BYte by byte*/
void uart_str_transmit(unsigned char *str)
{
    while(*str)
    {
        uart_char_transimit(*str++);
    }
}

/*Recieving the data byte by byte*/
unsigned char uart_receive(void)
{
    while(!RCIF);
    
    return RCREG;
}