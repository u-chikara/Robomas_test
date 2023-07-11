#include <CAN.h> //えすぺ内臓can

#define Motor_14 0x200
#define Motor_58 0x1FF

typedef struct
{
  int16_t m_id;
  int16_t mech_angle;
  int16_t rot_speed;
  int16_t current;
  uint8_t temp;
} motor_info;

typedef struct
{ // 足回り構造体
  int16_t motor0 = 0;
  int16_t motor1 = 0;
  int16_t motor2 = 0;
  int16_t motor3 = 0;
  float offrot=4/PI;
  float Rorad;
  uint16_t m_id;
} ashimawari;


extern unsigned short power_par[4];

//void motor_begin();

//void motor_rec(int packetSize);

int motor_move(int16_t id, int16_t m1, int16_t m2, int16_t m3, int16_t m4);

int BLmotor_move(int16_t id, int16_t m1, int16_t m2, int16_t m3, int16_t m4);