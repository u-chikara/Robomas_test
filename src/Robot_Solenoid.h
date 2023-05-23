#include <CAN.h> //えすぺ内臓can

extern hw_timer_t * timer;


typedef struct {
    unsigned short id;
    boolean switches[7]={0};
}Solenoid;

typedef struct {
    //自動的に切れる時間
    unsigned short pistim1;
    unsigned short pistim2;
    unsigned short pistim3;
    unsigned short pistim4;
    //タイマー加算時間
    unsigned short value1;
    unsigned short value2;
    unsigned short value3;
    unsigned short value4;
    //フラグ
    signed char pisenable1;
    signed char  pisenable2;
    signed char  pisenable3;
    signed char  pisenable4;

}S_safety;

extern S_safety ss;

void Solenoid_move(Solenoid sn);

void IRAM_ATTR PistonSafety();

void Solenoid_init();