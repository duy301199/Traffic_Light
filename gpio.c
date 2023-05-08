/*
 * gpio.c
 *
 *  Created on: Oct 10, 2022
 *      Author: duy301199
 */

#include <stdint.h>
#include "msp432p401r.h"

//GPIO Output P4.0-P4.5

void Traffic_light(void) {
        P4->SEL0 &= ~0x3F; // Using port P4.0 to P4.5
        P4->SEL1 &= ~0x3F;
        P4->DIR  |=  0x3F;
        P4->OUT  ^=  0x3F;
}

//GPIO Input P6.0-P6.1

void ControlTraffic(void) {
        P6->SEL0 &= ~0x03;
        P6->SEL1 &= ~0x03;
        P6->DIR  &= ~0x03;
}

//GPIO Input P1.1 switch

void ControlWalking(void){


        P1->SEL0 &= ~0x02;
        P1->SEL1 &= ~0x02;
        P1->REN  |=  0x02; // Pull up resistor
        P1->DIR  &= ~0x02;
}

//GPIOOutput P2.1-P2.0


void Walking_light(void) {
        P2->SEL0 &= ~0x07;
        P2->SEL1 &= ~0x07;
        P2->DIR  |=  0x07;
        P2->OUT  ^=  0x07;
}




