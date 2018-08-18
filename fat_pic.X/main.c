/*
 * File:   main.c
 * Author: alexm
 *
 * Created on July 17, 2018, 7:16 PM
 */

#define _XTAL_FREQ 4000000
#include "mcp_2515.h"
#include "config.h"
#include "plib.h"
#include <stdint.h>

#define LED_1_ON (LATD1 = 0)
#define LED_1_OFF (LATD1 = 1)
#define LED_2_ON (LATD2 = 0)
#define LED_2_OFF (LATD2 = 1)

static void interrupt rcv_test() {
    LED_1_ON;
    __delay_ms(100);
    LED_1_OFF;

    if (INTCON3bits.INT2IF) { 
        INTCON3bits.INT2IF = 0;
        
        // clear interrupts
        mcp_write_reg(CANINTF, 0);
    }
}

void main(void) {
    // set up clock
    OSCCONbits.IRCF = 0x6;  // internal osc freq sel: 4 MHz
    OSCCONbits.SCS = 0x2;   // system clock sel

    // set up LEDs
    TRISD1 = 0;
    TRISD2 = 0;
    LED_1_OFF;
    LED_2_OFF;
    
    // sync mode, bus mode, phase
    OpenSPI(SPI_FOSC_64, MODE_11, SMPMID);
   
    // XC8 doesn't support designated initializers
    can_t can_params;
    can_params.brp = 0;               // tq = 2 us
    can_params.sjw = 0x3;             // sjw = 4 tq
    can_params.btlmode = 0x1;         // ph2seg freely programmable
    can_params.sam = 0;               // sample once per bit
    can_params.seg1ph = 0x4;          // tseg1 = 5 tq
    can_params.prseg1 = 0x0;          // tprseg1 = 1 tq
    can_params.seg2ph = 0x4;          // tseg2 = 5 tq
    mcp_can_init(&can_params);
    
    // set up interrupts - move these to can init later...
    RCONbits.IPEN = 0;      // disable priority based interrupts
    INTCON3bits.INT2IF = 0;
    INTCON2bits.INTEDG2 = 0; // interrupt on falling edge
    INTCON3bits.INT2IE = 1;
    INTCONbits.GIE = 1;     // enable global interrupts

    uint8_t data_0[] = {0xca, 0xfe, 0xba, 0xbe};
    uint8_t data_1[] = {0xde, 0xad, 0xbe, 0xef};
    while (1) {
        mcp_can_send(0x2aa, data_0, 4);
        __delay_ms(1000);
        mcp_can_send(0x444, data_1, 4);
        __delay_ms(1000);
    }
    return;
}
