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

#define LED_ON (TRISD2 = 0)
#define LED_OFF (TRISD2 = 1)

void main(void) {
    // set up clock
    OSCCONbits.IRCF = 0x6;  // internal osc freq sel: 4 MHz
    OSCCONbits.SCS = 0x2;   // system clock sel
    
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
    can_params.seg2ph = 0x4;           // tseg2 = 5 tq
    mcp_can_init(&can_params);

    mcp_read_reg(TXB0CTRL);     // txb0ctrl
    mcp_read_reg(EFLG); // eflg

    uint8_t data0[] = {0xca, 0xfe, 0xba, 0xbe};
    uint8_t data1[] = {0xde, 0xad, 0xbe, 0xef};

    while (1) {
        mcp_can_send(0x2aa, data0, 4);
        __delay_ms(1000);
        
        mcp_can_send(0x444, data1, 4);
        __delay_ms(1000);
    }
    return;
}
