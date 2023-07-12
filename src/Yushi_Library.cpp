#ifndef YUSHI_LIB_CPP
#define YUSHI_LIB_CPP
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include <Yushi_Library.h>

int mintx = 353, maxtx = 3720, minty = 432, maxty = 3870;
unsigned char tbt = 0;
int tx = 0, ty = 0;


const unsigned char UC_font[13][14]{
    // 01234567
    {B00000000,  // 0
     B00000000,  // 1
     B00000000,  // 2
     B00000000,  // 3
     B00000000,  // 4
     B00000000,  // 5
     B01111110,  // 6
     B01111110,  // 7
     B00000000,  // 8
     B00000000,  // 9
     B00000000,  // 10
     B00000000,  // 11
     B00000000,  // 12
     B00000000}, // 13

    // 01234567
    {B00000000,  // 0
     B00000000,  // 1
     B00000000,  // 2
     B00000000,  // 3
     B00000000,  // 4
     B00000000,  // 5
     B00000000,  // 6
     B00000000,  // 7
     B00000000,  // 8
     B00000000,  // 9
     B00000000,  // 10
     B00000000,  // 11
     B00011000,  // 12
     B00011000}, // 13

    // 01234567
    {B00000010,  // 0
     B00000110,  // 1
     B00000110,  // 2
     B00000100,  // 3
     B00000100,  // 4
     B00001100,  // 5
     B00011000,  // 6
     B00011000,  // 7
     B00110000,  // 8
     B00100000,  // 9
     B00100000,  // 10
     B01100000,  // 11
     B01100000,  // 12
     B01000000}, // 13

    // 01234567
    {B00011000,  // 0
     B00100100,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B01100110,  // 4
     B01100110,  // 5
     B01100110,  // 6
     B01100110,  // 7
     B01100110,  // 8
     B01100110,  // 9
     B01100110,  // 10
     B01100110,  // 11
     B00100100,  // 12
     B00011000}, // 13

    // 01234567
    {B00011000,  // 0
     B00011000,  // 1
     B00111000,  // 2
     B01111000,  // 3
     B00011000,  // 4
     B00011000,  // 5
     B00011000,  // 6
     B00011000,  // 7
     B00011000,  // 8
     B00011000,  // 9
     B00011000,  // 10
     B00011000,  // 11
     B00011000,  // 12
     B01111110}, // 13

    // 01234567
    {B00111100,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B00000110,  // 4
     B00000110,  // 5
     B00001100,  // 6
     B00001000,  // 7
     B00011000,  // 8
     B00110000,  // 9
     B00110000,  // 10
     B01100000,  // 11
     B01100010,  // 12
     B01111110}, // 13

    // 01234567
    {B00111100,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B00000110,  // 4
     B00011110,  // 5
     B00011100,  // 6
     B00000110,  // 7
     B00000110,  // 8
     B00000110,  // 9
     B01100110,  // 10
     B01100110,  // 11
     B01100110,  // 12
     B00111100}, // 13

    // 01234567
    {B00001100,  // 0
     B00011100,  // 1
     B00011100,  // 2
     B00111100,  // 3
     B00101100,  // 4
     B00101100,  // 5
     B01101100,  // 6
     B01101100,  // 7
     B01001100,  // 8
     B01111110,  // 9
     B00001100,  // 10
     B00001100,  // 11
     B00001100,  // 12
     B00001100}, // 13

    // 01234567
    {B01111110,  // 0
     B01100000,  // 1
     B01100000,  // 2
     B01100000,  // 3
     B01100000,  // 4
     B01111100,  // 5
     B00000110,  // 6
     B00000110,  // 7
     B00000110,  // 8
     B00000110,  // 9
     B01100110,  // 10
     B01100110,  // 11
     B01100110,  // 12
     B00111100}, // 13

    // 01234567
    {B00111100,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B01100000,  // 4
     B01100000,  // 5
     B01111100,  // 6
     B01100110,  // 7
     B01100110,  // 8
     B01100110,  // 9
     B01100110,  // 10
     B01100110,  // 11
     B01100110,  // 12
     B00111100}, // 13

    // 01234567
    {B01111110,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B00000110,  // 4
     B00001100,  // 5
     B00001100,  // 6
     B00001100,  // 7
     B00001100,  // 8
     B00011000,  // 9
     B00011000,  // 10
     B00011000,  // 11
     B00011000,  // 12
     B00011000}, // 13

    // 01234567
    {B00111100,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B01100110,  // 4
     B00111100,  // 5
     B00011000,  // 6
     B00100100,  // 7
     B01100110,  // 8
     B01100110,  // 9
     B01100110,  // 10
     B01100110,  // 11
     B01100110,  // 12
     B00111100}, // 13

    // 01234567
    {B00111100,  // 0
     B01100110,  // 1
     B01100110,  // 2
     B01100110,  // 3
     B01100110,  // 4
     B01100110,  // 5
     B00111110,  // 6
     B00000110,  // 7
     B00000110,  // 8
     B00001100,  // 9
     B00001100,  // 10
     B00001100,  // 11
     B00111000,  // 12
     B00110000}, // 13
};

void CreateButton(Button_t *bt, int x, int y, unsigned char dx, unsigned char dy, unsigned char textsize, const char *word)
{
  bt->x = x;
  bt->y = y;
  bt->dx = dx;
  bt->dy = dy;
  bt->words = strlen(word);
  bt->config = textsize & 0xf;
  memset(bt->word, '\0', 24);
  memcpy(bt->word, word, 24);
  return;
}

void DrawButton(Button_t bt, unsigned char push)
{
  unsigned char db;

  tft.fillRect(bt.x + 2, bt.y + 2, bt.dx - 4, bt.dy - 4, 0xf79e);

  if (push)
  {
    for (db = 0; db < bt.dx; db++)
    {
      tft.drawPixel(bt.x + db, bt.y, 0xd69a);
      tft.drawPixel(bt.x + db, bt.y + 1, 0xa514);

      tft.drawPixel(bt.x + db, bt.y + bt.dy - 2, 0xe71c);
      tft.drawPixel(bt.x + db, bt.y + bt.dy - 1, 0xffff);
    }

    for (db = 0; db < bt.dy - 2; db++)
    {
      tft.drawPixel(bt.x, bt.y + db, 0xd69a);
      tft.drawPixel(bt.x + 1, bt.y + db + 1, 0xa514);

      tft.drawPixel(bt.x + bt.dx - 2, bt.y + db + 2, 0xe71c);
      tft.drawPixel(bt.x + bt.dx - 1, bt.y + db + 2, 0xffff);
    }
  }
  else
  {
    for (db = 0; db < bt.dx; db++)
    {
      tft.drawPixel(bt.x + db, bt.y, 0xffff);
      tft.drawPixel(bt.x + db, bt.y + 1, 0xe71c);

      tft.drawPixel(bt.x + db, bt.y + bt.dy - 2, 0xa514);
      tft.drawPixel(bt.x + db, bt.y + bt.dy - 1, 0xd69a);
    }

    for (db = 0; db < bt.dy - 2; db++)
    {
      tft.drawPixel(bt.x, bt.y + db, 0xffff);
      tft.drawPixel(bt.x + 1, bt.y + db + 1, 0xe71c);

      tft.drawPixel(bt.x + bt.dx - 2, bt.y + db + 2, 0xa514);
      tft.drawPixel(bt.x + bt.dx - 1, bt.y + db + 2, 0xd69a);
    }
  }
  tft.setCursor(bt.x + (bt.dx >> 1) - (bt.config & 0xf) * 4 - bt.words * (bt.config & 0xf) * 5 / 2 + push * 2, bt.y + (bt.dy >> 1) - (bt.config & 0xf) * 4 + push * 2);
  tft.setTextSize(bt.config & 0xf);
  tft.setTextColor(0);
  tft.print(bt.word);
}

void DrawButtonAll(Button_t *bt, unsigned char button_num)
{
  unsigned char bn;

  for (bn = 0; bn < button_num; bn++)
  {
    DrawButton(bt[bn], 0);
  }
  return;
}

unsigned char GetPanelPos(int *ppx, int *ppy)
{
  TS_Point p = ts.getPoint();
  if (ts.touched())
  {
    *ppx = (p.x - mintx) * 240 / (maxtx - mintx);
    *ppy = (p.y - minty) * 320 / (maxty - minty);
    return 1;
  }
  return 0;
}

signed char ButtonTouch(Button_t *bt, unsigned char button_num)
{
  signed char tcou;
  signed char oldc;

  if ((GetPanelPos(&tx, &ty) ^ tbt) != 0)
  {
    tbt = ts.touched();
    for (tcou = 0; tcou < button_num; tcou++)
    {
      
      if (bt[tcou].x < tx && bt[tcou].x + bt[tcou].dx > tx && bt[tcou].y < ty && bt[tcou].y + bt[tcou].dy > ty)
      {
        oldc=tcou;
        if (tbt)
        {
          DrawButton(bt[tcou], 1);
        }
        else
        {
          DrawButton(bt[tcou], 0);


          return tcou;
        }
      }else{

      }
      
    }
  }
  return -1;
}

void DrawUCfont(int x, int y, uint16_t color, char *data)
{

  unsigned char duc = 0;
  while (data[duc] != '\0')
  {
    for (unsigned char du = 0; du < 14; du++)
    {
      for (unsigned char d = 0; d < 8; d++)
      {
        if ((UC_font[data[duc] - '-'][du] >> d) & 1)
        {
          tft.drawPixel(x + 8 - d + duc * 8, y + du, color);
        }
      }
    }
    duc++;
  }

  return;
}

void CreateScrollbar(Scrollbar_t *sb, int x, int y, int barsize, unsigned char tglsize)
{
  sb->x = x;
  sb->y = y;
  sb->barsize = barsize;
  sb->tglsize = tglsize;
  return;
}

void DrawScrollbarAll(Scrollbar_t *sb, unsigned char scrollbar_num)
{
  unsigned char sn;

  for (sn = 0; sn < scrollbar_num; sn++)
  {
    DrawScrollbar(sb[sn]);
  }
  return;
}

void DrawScrollbar(Scrollbar_t sb)
{
  tft.fillRect(sb.x-sb.tglsize-3,sb.y-sb.tglsize-3,sb.barsize+sb.tglsize*2+6,sb.tglsize*2+6,0xf79e);
  tft.fillRect(sb.x, sb.y - 1, sb.barsize, 3, 0xa514);
  tft.fillCircle(sb.x + sb.value * sb.barsize / 0xffff, sb.y, sb.tglsize, 0xd69a);

  return;
}

signed char ScrollbarTouch(Scrollbar_t *sb, unsigned char scrollbar_num)
{
  unsigned char scou;
  if (GetPanelPos(&tx, &ty))
  {
    for (scou = 0; scou < scrollbar_num; scou++)
    {
      if (sb[scou].x < tx && sb[scou].x + sb[scou].barsize > tx && sb[scou].y - sb[scou].tglsize-3 < ty && sb[scou].y + sb[scou].tglsize+3 > ty)
      {
        sb[scou].value= (tx - sb[scou].x) *0xffff/ sb[scou].barsize;
        DrawScrollbar(sb[scou]);
        return scou;
      }
    }
  }
  return -1;
}

#endif