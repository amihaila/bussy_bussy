#ifndef LED_H_
#define LED_H_

#define LED_1_PORT      D
#define LED_2_PORT      D
#define LED_1_INDEX     5
#define LED_2_INDEX     6

#define _MAKE_PORT(port, index) LAT##port##index
#define _MAKE_TRIS(port, index) TRIS##port##index

#define MAKE_PORT(port, index) _MAKE_PORT(port, index)
#define MAKE_TRIS(port, index) _MAKE_TRIS(port, index)

#define LED_SETUP do { \
    MAKE_TRIS(LED_1_PORT, LED_1_INDEX) = 0; \
    MAKE_PORT(LED_1_PORT, LED_1_INDEX) = 0; \
    MAKE_TRIS(LED_2_PORT, LED_2_INDEX) = 0; \
    MAKE_PORT(LED_2_PORT, LED_2_INDEX) = 1; \
} while(0)

#define LED_1_ON MAKE_PORT(LED_1_PORT, LED_1_INDEX) = 0;
#define LED_1_OFF MAKE_PORT(LED_1_PORT, LED_1_INDEX) = 1; 
#define LED_2_ON MAKE_PORT(LED_2_PORT, LED_2_INDEX) = 0;
#define LED_2_OFF MAKE_PORT(LED_2_PORT, LED_2_INDEX) = 1; 

#endif
