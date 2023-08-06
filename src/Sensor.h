#include <Arduino.h>

typedef struct
{ // センサー基盤構造体
  uint16_t s_id1;
  uint16_t s_id2;
  uint64_t timestamp;
  uint16_t rotaen0;
  uint16_t rotaen1;
  uint16_t rotaen2;
  uint16_t rotaen3;
  uint16_t rotaen4;
  uint8_t rimit0; // つかむ
  uint8_t rimit1; // アーム昇降↑
  uint8_t rimit2; // アーム昇降↓
  uint8_t rimit3; // アーム展開
  uint8_t rimit4; // 仰角↓
} sensorkiban;

