#include "mcp_2515.h"
#include "plib.h"

//SPI command macros
#define RESET       0b11000000
#define READ        0b00000011
#define READ_RX_B0  0b10010000  // read rx buffer 0
#define WRITE       0b00000010
#define LD_TX_B0    0b01000000  // read tx buffer 0
#define RTS_B0      0b10000000  // request to send on buff 0
#define READ_STAT   0b10100000
#define RX_STAT     0b10110000
#define BIT_MOD     0b00000101

void mcp_can_init(can_t *can_params) {
    TRISD3 = 0;
    LATD3 = 1;

    // set to config mode. top 3 bits are 0b100
    mcp_write_reg(CANCTRL, 0x4 << 5);
    while (!(mcp_read_reg(CANCTRL))) { }

    mcp_write_reg(CNF1, can_params->sjw << 6 | can_params->brp);
    mcp_write_reg(CNF2, can_params->btlmode << 7 | can_params->sam << 6
        | can_params->seg1ph << 3 | can_params->prseg1);
    mcp_write_reg(CNF3, can_params->seg2ph);

    // set normal mode (top 3 bits = 0, set clock output)
    // set one shot mode
    mcp_write_reg(CANCTRL, 0xc);
    // mcp_write_reg(CANCTRL, 0x4);


    // normal mode: top 3 bits are 0
    while (mcp_read_reg(CANCTRL) & 0xe0 != 0);   // wait for normal mode
}


void mcp_can_send(uint16_t sid, uint8_t *data, uint8_t data_length) {
    mcp_write_reg(CANINTF, 0);     // clear interrupt flag register
    mcp_write_reg(EFLG, 0);

    mcp_write_reg(TXB0SIDH, (uint8_t) (sid >> 3));          // set sid
    mcp_write_reg(TXB0SIDL, (sid & 0x7) << 5);              // set sid

    // data registers are consecutive
    for (int i = 0; i < data_length; ++i) {
        mcp_write_reg(TXB0D0 + i, data[i]);
    }

    // data message w/ some number of bytes
    mcp_write_reg(TXB0DLC, data_length);
    mcp_write_reg(TXB0CTRL, 1 << 3);            // set txreq
}

void mcp_write_reg(uint8_t addr, uint8_t data) {
    LATD3 = 0;
    WriteSPI(WRITE);
    WriteSPI(addr);
    WriteSPI(data);
    LATD3 = 1;
}

uint8_t mcp_read_reg(uint8_t addr) {
    LATD3 = 0;
    WriteSPI(READ);
    WriteSPI(addr);
    uint8_t ret =  ReadSPI();
    LATD3 = 1;
    return ret;
}

