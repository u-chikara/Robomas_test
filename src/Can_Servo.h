#include <CAN.h> //えすぺ内臓can

typedef struct
{ // サーボ基板構造体
  uint16_t v_id = 0;
  int servo0 = 0;
  int servo1 = 0;
  int servo2 = 0;
  int servo3 = 0;
  int servo4 = 0;
  int servo5 = 0;
  int servo6 = 0;
  int servo7 = 0;
} savokiban;

int voba_move(savokiban sk);