#include <Can_Servo.h>

int voba_move(savokiban sk) // サーボデーター送信
{
  CAN.beginPacket(sk.v_id);
  CAN.write((unsigned char)sk.servo0);
  CAN.write((unsigned char)sk.servo1);
  CAN.write((unsigned char)sk.servo2);
  CAN.write((unsigned char)sk.servo3);
  CAN.write((unsigned char)sk.servo4);
  CAN.write((unsigned char)sk.servo5);
  CAN.write((unsigned char)sk.servo6);
  CAN.write((unsigned char)sk.servo7);
  return CAN.endPacket();
}