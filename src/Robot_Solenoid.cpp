#include <CAN.h> //えすぺ内臓can

#include <Robot_Solenoid.h>

hw_timer_t * timer=NULL;

S_safety ss;

void IRAM_ATTR PistonSafety(){
    if(ss.pisenable1!=0){
        ss.value1++;
    }
    if(ss.pisenable2!=0){
        ss.value2++;
    }
    if(ss.pisenable3!=0){
        ss.value3++;
    }
    if(ss.pisenable4!=0){
        ss.value4++;
    }

}

void Solenoid_init(){
    timer = timerBegin(0,getApbFrequency()/1000000,true);

    timerAttachInterrupt(timer, &PistonSafety, true);

    timerAlarmWrite(timer, 1000, true);

    timerAlarmEnable(timer);

    ss.pistim1=500;
    ss.pistim2=500;
    ss.pistim3=500;
    ss.pistim4=500;
    return;
}

void Solenoid_move(Solenoid sn)
{
    CAN.beginPacket(sn.id);
    CAN.write(sn.switches[0]|(sn.switches[1]<<1)|(sn.switches[2]<<2)|(sn.switches[3]<<3)|(sn.switches[4]<<4)|(sn.switches[5]<<5)|(sn.switches[6]<<6)|(sn.switches[7]<<7));
    CAN.endPacket();
    return;
}
