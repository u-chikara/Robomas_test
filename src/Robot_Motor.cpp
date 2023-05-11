#include <Robot_Motor.h>

#include <CAN.h> //えすぺ内臓can

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
  if (m1 > 0x3fff)
    m1 = 0x3fff;
  if (m1 < -0x3fff)
    m1 = -0x3fff;
  if (m2 > 0x3fff)
    m2 = 0x3fff;
  if (m2 < -0x3fff)
    m2 = -0x3fff;
  if (m3 > 0x3fff)
    m3 = 0x3fff;
  if (m3 < -0x3fff)
    m3 = -0x3fff;
  if (m4 > 0x3fff)
    m4 = 0x3fff;
  if (m4 < -0x3fff)
    m4 = -0x3fff;

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
