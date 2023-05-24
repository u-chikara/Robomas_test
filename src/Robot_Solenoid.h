#include <CAN.h> //えすぺ内臓can

extern hw_timer_t * timer;


typedef struct {
    unsigned short id;
}Solenoid;

void Solenoid_move(unsigned char sm0,unsigned char sm1,unsigned char sm2,unsigned char sm3,unsigned char sm4,unsigned char sm5,unsigned char sm6,unsigned char sm7);

void Cylinder_offtime(unsigned short co0,unsigned short co1,unsigned short co2,unsigned short co3);

void Cylinder_move(unsigned char cm0,unsigned char cm1,unsigned char cm2,unsigned char cm3);