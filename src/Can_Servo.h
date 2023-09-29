#include <CAN.h> //えすぺ内臓can

typedef struct
{ // サーボ基板構造体
  uint16_t v_id;
  int servo0;
  int servo1;
  int servo2;
  int servo3;
  int servo4;
  int servo5;
  int servo6;
  int servo7;
} savokiban;

int voba_move(savokiban sk);