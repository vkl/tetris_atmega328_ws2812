/*
 * coords.h
 *
 * Created: 3/13/2022 4:16:00 PM
 *  Author: vklad
 */ 


#ifndef COORDS_H_
#define COORDS_H_

#include "common.h"
#include "ws2812b.h"

#define GETX(N) (ledCoords[N] & 0b1111)
#define GETY(N) ((ledCoords[N] >> 4) & 0b11111)

uint16_t ledCoords[PIXELS] = {
    0x130,
    0x131,
    0x132,
    0x133,
    0x134,
    0x124,
    0x123,
    0x122,
    0x121,
    0x120,
    0x110,
    0x111,
    0x112,
    0x113,
    0x114,
    0x104,
    0x103,
    0x102,
    0x101,
    0x100,
    0x0F0,
    0x0F1,
    0x0F2,
    0x0F3,
    0x0F4,
    0x0E4,
    0x0E3,
    0x0E2,
    0x0E1,
    0x0E0,
    0x0D0,
    0x0D1,
    0x0D2,
    0x0D3,
    0x0D4,
    0x0C4,
    0x0C3,
    0x0C2,
    0x0C1,
    0x0C0,
    0x0B0,
    0x0B1,
    0x0B2,
    0x0B3,
    0x0B4,
    0x0A4,
    0x0A3,
    0x0A2,
    0x0A1,
    0x0A0,
    0x090,
    0x091,
    0x092,
    0x093,
    0x094,
    0x084,
    0x083,
    0x082,
    0x081,
    0x080,
    0x070,
    0x071,
    0x072,
    0x073,
    0x074,
    0x064,
    0x063,
    0x062,
    0x061,
    0x060,
    0x050,
    0x051,
    0x052,
    0x053,
    0x054,
    0x044,
    0x043,
    0x042,
    0x041,
    0x040,
    0x030,
    0x031,
    0x032,
    0x033,
    0x034,
    0x024,
    0x023,
    0x022,
    0x021,
    0x020,
    0x010,
    0x011,
    0x012,
    0x013,
    0x014,
    0x004,
    0x003,
    0x002,
    0x001,
    0x000,
    0x009,
    0x008,
    0x007,
    0x006,
    0x005,
    0x015,
    0x016,
    0x017,
    0x018,
    0x019,
    0x029,
    0x028,
    0x027,
    0x026,
    0x025,
    0x035,
    0x036,
    0x037,
    0x038,
    0x039,
    0x049,
    0x048,
    0x047,
    0x046,
    0x045,
    0x055,
    0x056,
    0x057,
    0x058,
    0x059,
    0x069,
    0x068,
    0x067,
    0x066,
    0x065,
    0x075,
    0x076,
    0x077,
    0x078,
    0x079,
    0x089,
    0x088,
    0x087,
    0x086,
    0x085,
    0x095,
    0x096,
    0x097,
    0x098,
    0x099,
    0x0A9,
    0x0A8,
    0x0A7,
    0x0A6,
    0x0A5,
    0x0B5,
    0x0B6,
    0x0B7,
    0x0B8,
    0x0B9,
    0x0C9,
    0x0C8,
    0x0C7,
    0x0C6,
    0x0C5,
    0x0D5,
    0x0D6,
    0x0D7,
    0x0D8,
    0x0D9,
    0x0E9,
    0x0E8,
    0x0E7,
    0x0E6,
    0x0E5,
    0x0F5,
    0x0F6,
    0x0F7,
    0x0F8,
    0x0F9,
    0x109,
    0x108,
    0x107,
    0x106,
    0x105,
    0x115,
    0x116,
    0x117,
    0x118,
    0x119,
    0x129,
    0x128,
    0x127,
    0x126,
    0x125,
    0x135,
    0x136,
    0x137,
    0x138,
    0x139
};


#endif /* COORDS_H_ */