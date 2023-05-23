#include <Robot_Motor.h>

#include <CAN.h> //えすぺ内臓can

unsigned short power_par[4]={10000,10000,10000,10000};

int BLmotor_move(int16_t id, int16_t m1, int16_t m2, int16_t m3, int16_t m4)
{ // モーターデーター送信
  CAN.beginPacket(id);
  CAN.write(m1 >> 8);
  CAN.write(m1 & 0xff);
  CAN.write(m2 >> 8);
  CAN.write(m2 & 0xff);
  CAN.write(m3 >> 8);
  CAN.write(m3 & 0xff);
  CAN.write(m4 >> 8);
  CAN.write(m4 & 0xff);
  return CAN.endPacket();
}

int motor_move(int16_t id, int16_t m1, int16_t m2, int16_t m3, int16_t m4) // モーターデーター送信
{
  if (m1 > power_par[0])
    m1 = power_par[0];
  if (m1 < -power_par[0])
    m1 = -power_par[0];
  if (m2 > power_par[1])
    m2 = power_par[1];
  if (m2 < -power_par[1])
    m2 = -power_par[1];
  if (m3 > power_par[2])
    m3 = power_par[2];
  if (m3 < -power_par[2])
    m3 = -power_par[2];
  if (m4 > power_par[3])
    m4 = power_par[3];
  if (m4 < -power_par[3])
    m4 = -power_par[3];

  CAN.beginPacket(id);
  CAN.write(m1 & 0xff);
  CAN.write(m1 >> 8);
  CAN.write(m2 & 0xff);
  CAN.write(m2 >> 8);
  CAN.write(m3 & 0xff);
  CAN.write(m3 >> 8);
  CAN.write(m4 & 0xff);
  CAN.write(m4 >> 8);
  return CAN.endPacket();
}
