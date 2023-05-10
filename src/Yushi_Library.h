#pragma once

// #ifndef YUSHI_LIB_H
// #define YUSHI_LIB_H
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

typedef struct
{
  int x;
  int y;
  unsigned char dx;
  unsigned char dy;
  unsigned char words;
  char word[24];

  unsigned char config;
} Button_t;

typedef struct{
  int x;
  int y;
  int barsize;
  unsigned char tglsize;
  unsigned char value;
} Scrollbar_t;

#define TFT_CS 4
#define TFT_RST 16
#define TFT_DC 5

inline XPT2046_Touchscreen ts(2);
inline Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); 

extern int mintx, maxtx, minty, maxty;
extern unsigned char tbt;
extern int tx, ty;

void CreateButton(Button_t *bt, int x, int y, unsigned char dx, unsigned char dy, unsigned char textsize, const char *word);

void DrawButton(Button_t bt, unsigned char push);

void DrawButtonAll(Button_t *bt, unsigned char button_num);

unsigned char GetPanelPos(int *ppx,int *ppy);

signed char ButtonTouch(Button_t *bt, unsigned char button_num);

void DrawUCfont(int x, int y, uint16_t color, char *data);

void CreateScrollbar(Scrollbar_t *sb, int x, int y, int barsize, unsigned char tglsize);

void DrawScrollbarAll(Scrollvar_t *sb, unsigned char scrollbar_num);

void DrawScrollbar(Scrollbar_t sb);

// #endif