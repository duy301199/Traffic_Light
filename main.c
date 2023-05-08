#include <stdint.h>
#include "SysTick.h"
#include "msp432p401r.h"

#include "gpio.h"

struct State {
    uint32_t Out[2];
    uint32_t Time;
    const struct State *Next[14];
};

//   FSM States

typedef const struct State State_t;
#define goN   &FSM[0]           // Green on North
#define waitN &FSM[1]           // Wait on North
#define goE   &FSM[2]           // Green on East
#define waitE &FSM[3]           // Wait on East

#define walkRequest  &FSM[4]    //Request for a walk.
#define walkG &FSM[5]           // Walking Green on

#define walkF1 &FSM[6]          // FlashingOn
#define walkF2 &FSM[7]          // FlashingOff
#define walkF3 &FSM[8]          // FlashingOn
#define walkF4 &FSM[9]          // FlashingOff
#define walkF5 &FSM[10]         // FlashingOn
#define walkF6 &FSM[11]         // FlashingOff
#define walkF7 &FSM[12]

#define walkR &FSM[13]          // Resume Traffic



//States

State_t FSM[14] = {   // TLight,Wlight,Delay       000      001     010     011         100          101         110          111
/*goN*/                 {{0x0C, 0x01}  ,200,      {goN,     waitN,  goN,    waitN,  walkRequest, walkRequest, walkRequest, walkRequest}},
/*waitN*/               {{0x14, 0x01}  ,80,       {goE,     goE,    goE,    goE,    walkRequest, walkRequest, walkRequest, walkRequest}},
/*goE*/                 {{0x21, 0x01}  ,200,      {goE,     goE,    waitE,  waitE,  walkRequest, walkRequest, walkRequest, walkRequest}},
/*waitE*/               {{0x22, 0x01}  ,80,       {goN,     goN,    goN,    goN,    walkRequest, walkRequest, walkRequest, walkRequest}},

/*walkRequest*/         {{0x24, 0x02},  150,      {walkG,   walkG,  walkG,  walkG,     walkG,      walkG,       walkG,       walkG}},

/*walkG*/               {{0x24, 0x01},  50,       {walkF1,  walkF1, walkF1, walkF1,    walkF1,     walkF1,      walkF1,      walkF1}},
/*StartFlash*/          {{0x24, 0x00},  50,       {walkF2,  walkF2, walkF2, walkF2,    walkF2,     walkF2,      walkF2,      walkF2}},
                        {{0x24, 0x01},  50,       {walkF3,  walkF3, walkF3, walkF3,    walkF3,     walkF3,      walkF3,      walkF3}},
                        {{0x24, 0x00},  50,       {walkF4,  walkF4, walkF4, walkF4,    walkF4,     walkF4,      walkF4,      walkF4}},
                        {{0x24, 0x01},  50,       {walkF5,  walkF5, walkF5, walkF5,    walkF5,     walkF5,      walkF5,      walkF5}},
                        {{0x24, 0x00},  50,       {walkF6,  walkF6, walkF6, walkF6,    walkF6,     walkF6,      walkF6,      walkF6}},
                        {{0x24, 0x01},  50,       {walkF7,  walkF7, walkF7, walkF7,    walkF7,     walkF7,      walkF7,      walkF7}},
/*EndFlash*/            {{0x24, 0x00},  50,       {walkR,   walkR,  walkR,  walkR,     walkR,      walkR,       walkR,       walkR}},
/*walkR,resumetraffic*/ {{0x24, 0x01},  80,       {goN,     goN,    goE,    goE,        goN,        goN,         goE,          goN}},

};




int main(void) {

    State_t *Pt;
    uint32_t Control_traffic,Control_walk;


    SysTick_Init();
    ControlWalking();
    Walking_light();
    Traffic_light();
    ControlTraffic();


    Pt = goN;   //Initial state is Green north Red east

    while(1){
        P4->OUT = (P4->OUT&~0x3F)|(Pt->Out[0]);
        P2->OUT = (P2->OUT&~0x07)|(Pt->Out[1]);

        SysTick_Wait10ms(Pt->Time);

        Control_traffic = (P6->IN&0x03);


        Control_walk =  P1->IN;
        Control_walk =  ((~Control_walk)&0x02)<<1;
                   Control_traffic |= Control_walk;
                   Pt = Pt->Next[Control_traffic];
       }
    }

