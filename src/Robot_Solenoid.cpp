#include <CAN.h> //えすぺ内臓can

#include <Robot_Solenoid.h>


void Solenoid_move(unsigned char sm0,unsigned char sm1,unsigned char sm2,unsigned char sm3,unsigned char sm4,unsigned char sm5,unsigned char sm6,unsigned char sm7)
{
    CAN.beginPacket(0xf1);
    CAN.write(sm0);
    CAN.write(sm1);
    CAN.write(sm2);
    CAN.write(sm3);
    CAN.write(sm4);
    CAN.write(sm5);
    CAN.write(sm6);
    CAN.write(sm7);
    CAN.endPacket();
    return;
}

void Cylinder_offtime(unsigned short co0,unsigned short co1,unsigned short co2,unsigned short co3){
    CAN.beginPacket(0xf3);
    CAN.write(co0>>8);
    CAN.write(co0&0xff);
    CAN.write(co1>>8);
    CAN.write(co1&0xff);
    CAN.write(co2>>8);
    CAN.write(co2&0xff);
    CAN.write(co3>>8);
    CAN.write(co3&0xff);
    CAN.endPacket();
    return;
}

void Cylinder_move(unsigned char cm0,unsigned char cm1,unsigned char cm2,unsigned char cm3){
    CAN.beginPacket(0xf2);
    CAN.write(cm0);
    CAN.write(cm1);
    CAN.write(cm2);
    CAN.write(cm3);
    CAN.write(0);
    CAN.write(0);
    CAN.write(0);
    CAN.write(0);
    CAN.endPacket();
    return;
}