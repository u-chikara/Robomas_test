//Copy Right 2023 U.Chikara

#include <PS4Controller.h>
#include <Robot_Motor.h>
#include <Robot_Solenoid.h>
#include <Yushi_Library.h>


void Menu();
void BlessMotor();
void LCD_Config();

Solenoid SN;

Button_t butt[12];

Scrollbar_t sbar[10];

ashimawari AM;

motor_info moin;

float tyousei = 0.25;
int16_t motor = 0;
int16_t rpm = 0;
char pushc;

signed short mm1,mm2,mm3,mm4;

char buf[16];


void Piston_move(){
  if(ss.pisenable1!=0){
    
    if(ss.pisenable1==1){
      SN.switches[0]=1;
      SN.switches[1]=0;
    }
    if(ss.pisenable1==-1){
      SN.switches[0]=0;
      SN.switches[1]=1;
    }

    if(ss.value1>ss.pistim1){
      ss.pisenable1=0;
      ss.value1=0;
      SN.switches[0]=0;
      SN.switches[1]=0;
    }
    printf("%d,%d\n",ss.pisenable1,ss.value1);
  }

  
  Solenoid_move(SN);
  return;
}

void SolenoidTest(){//ソレノイド基盤
  tft.fillScreen(0xf79e);
  CreateButton(&butt[0], 190, 290, 50, 30, 1, "Back");

  char spc[5]; 

  spc[0]='S';
  spc[1]='p';
  spc[2]='_';
  spc[4]='\0';
  
  unsigned char spb;

  for(spb=0;spb<8;spb++){
    spc[3]='1'+spb;
    CreateButton(&butt[spb+1],0,spb*40,50,40,1,spc);
  }

  CreateButton(&butt[9],100,0,50,40,1,"Pis1_Ext");
  CreateButton(&butt[10],100,40,50,40,1,"Pis1_Shr");

  DrawButtonAll(butt, 11);

  while(1){
    pushc = ButtonTouch(butt, 11);
    if(pushc==0)Menu();
    if(pushc>0){
      if(pushc==1)SN.switches[0]=!SN.switches[0];
      if(pushc==2)SN.switches[1]=!SN.switches[1];
      if(pushc==3)SN.switches[2]=!SN.switches[2];
      if(pushc==4)SN.switches[3]=!SN.switches[3];
      if(pushc==5)SN.switches[4]=!SN.switches[4];
      if(pushc==6)SN.switches[5]=!SN.switches[5];
      if(pushc==7)SN.switches[6]=!SN.switches[6];
      if(pushc==8)SN.switches[7]=!SN.switches[7];
      if(pushc==9)ss.pisenable1=1;
      if(pushc==10)ss.pisenable1=-1;

      for(spb=0;spb<8;spb++){
        tft.fillCircle(60,20+spb*40,7,ST77XX_BLUE+0xf7e1*SN.switches[spb]);
      }
      Piston_move();
    }
    Solenoid_move(SN);
    delay(16);
  }
  
  Solenoid_move(SN);
  Menu();
  return;
}

void Mecanum(ashimawari am){//メカナム処理
  if (PS4.isConnected()){
    if(PS4.LStickX()>10 or PS4.LStickX()<-10 or PS4.LStickY()>10 or PS4.LStickY()<-10){
      am.Rorad=atan2(PS4.LStickY(),PS4.LStickX());
      am.motor0=sin(am.Rorad+PI/4+am.offrot)*power_par[0];
      am.motor1=sin(am.Rorad+PI*3/4+am.offrot)*power_par[1];
      am.motor2=sin(am.Rorad+PI*5/4+am.offrot)*power_par[2];
      am.motor3=sin(am.Rorad+PI*7/4+am.offrot)*power_par[3];
      BLmotor_move(Motor_14,am.motor0,am.motor1,am.motor2,am.motor3);
    }else{
      BLmotor_move(Motor_14,0,0,0,0);
    }
  }
  return;
}

void can_rec(int packetSize)//CAN割り込み受信
{ // センサーデーター受信
  if (CAN.packetId() == moin.m_id)
  {
    moin.mech_angle = CAN.read() << 8 | CAN.read();
    moin.rot_speed = CAN.read() << 8 | CAN.read();
    moin.current = CAN.read() << 8 | CAN.read();
    moin.temp = CAN.read();
  }
}

void can_begin(){//CAN初期化
    CAN.setPins(17, 21); //( rx , tx )
    CAN.begin(1E6);
    CAN.onReceive(can_rec);
    return;
}

void RobotProcess(){//ロボット全体処理
  if (PS4.isConnected())
    {

      if (PS4.LStickY() > 10 or PS4.LStickY() < -10)
        rpm += PS4.LStickY();

      if (PS4.Circle())
        rpm = 0;

      if (PS4.Up())
        rpm++;
      if (PS4.Down())
        rpm--;
      if (PS4.Left())
        rpm -= 10;
      if (PS4.Right())
        rpm += 10;

      if (PS4.L1() and tyousei>0)
        tyousei -= 0.05;
      if (PS4.R1() and tyousei<1)
        tyousei += 0.05;

      // if(abs(rpm)>1000 and abs(moin.rot_speed)<10){
      //   ERROR();
      // }

      motor += (rpm - moin.rot_speed) * tyousei;

      BLmotor_move(Motor_58, motor, 0, 0, 0);
      
    }
  
    return;
}

void BlessMotor()//ブラシレスモーター
{
  CreateButton(&butt[0], 0, 160, 30, 30, 1, "MT_1");
  CreateButton(&butt[1], 30, 160, 30, 30, 1, "MT_2");
  CreateButton(&butt[2], 60, 160, 30, 30, 1, "MT_3");
  CreateButton(&butt[3], 90, 160, 30, 30, 1, "MT_4");
  CreateButton(&butt[4], 120, 160, 30, 30, 1, "MT_5");
  CreateButton(&butt[5], 150, 160, 30, 30, 1, "MT_6");
  CreateButton(&butt[6], 180, 160, 30, 30, 1, "MT_7");
  CreateButton(&butt[7], 210, 160, 30, 30, 1, "MT_8");
  CreateButton(&butt[8], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 9);

  unsigned char grx;

  while(1){
    //RobotProcess();
    //青線表示
    tft.drawLine(0, 119, 239, 119, ST77XX_BLUE);
    tft.drawLine(119, 0, 119, 119, ST77XX_BLUE);

    // RPMグラフ表示
    tft.drawLine(grx, 0, grx, 118, ST77XX_BLACK);
    tft.drawPixel(grx, 63 - moin.rot_speed / 153, ST77XX_GREEN);
    tft.drawPixel(grx, 63 - rpm / 153, ST77XX_CYAN);
    if(grx&1)tft.drawPixel(grx,32,ST77XX_BLUE);
    if(grx&1)tft.drawPixel(grx,92,ST77XX_BLUE);

    // 電流グラフ表示
    tft.drawLine(grx + 120, 0, grx + 120, 118, ST77XX_BLACK);
    tft.drawPixel(grx + 120, 63 - moin.current / 157, ST77XX_YELLOW);

    grx++;
    if (grx > 118)
      grx = 0;

    // RPM表示
    tft.fillRect(0, 0, 48, 14, 0);
    sprintf(buf, "%04d", moin.rot_speed);
    DrawUCfont(0, 0, ST77XX_RED, buf);

    // 電流表示
    tft.fillRect(120, 0, 48, 14, 0);
    sprintf(buf, "%04d", moin.current);
    DrawUCfont(120, 0, ST77XX_RED, buf);

    // motor値
    tft.fillRect(0, 120, 48, 14, 0xf79e);
    sprintf(buf, "%05d", motor);
    DrawUCfont(0, 120, ST77XX_RED, buf);

    // rpm値
    tft.fillRect(0, 120 + 16, 48, 14, 0xf79e);
    sprintf(buf, "%05d", rpm);
    DrawUCfont(0, 120 + 16, ST77XX_RED, buf);

    //tyousei値
    tft.fillRect(120, 120 , 80, 14, 0xf79e);
    sprintf(buf, "%f", tyousei);
    DrawUCfont(120, 120, ST77XX_RED, buf);

    pushc = ButtonTouch(butt, 9);

    if(pushc==8)Menu();

    delay(16);
  }

  return;
}

void Bmotor(){//ブラシモーター
  unsigned char bmb=0;

  char bmc[8];

  bmc[0]='M';
  bmc[2]='\0';

  tft.fillScreen(0xf79e);

  CreateButton(&butt[0], 0, 290, 50, 30, 1, "Back");
  CreateButton(&butt[9], 160, 278, 40, 40, 1, "<-");
  CreateButton(&butt[10], 200, 278, 40, 40, 1, "->");

  tft.setTextColor(0);

  for(bmb=0;bmb<4;bmb++){
    tft.setCursor(210,10+bmb*70);
    bmc[1]='1'+bmb;
    tft.print(bmc);
    tft.drawRect(2,2+bmb*70,235,64,0);
    CreateButton(&butt[1+bmb], 185, 40+bmb*70, 50, 20, 1, "STOP");
    CreateButton(&butt[5+bmb], 135, 40+bmb*70, 50, 20, 1, "Unity");
    CreateScrollbar(&sbar[bmb*2],16,32+bmb*70,200,7);
    CreateScrollbar(&sbar[1+bmb*2],16,50+bmb*70,100,7);
    sbar[1+bmb*2].value=power_par[bmb]*2;
  }

  sbar[0].value=0x7fff;
  sbar[2].value=0x7fff;
  sbar[4].value=0x7fff;
  sbar[6].value=0x7fff;

  DrawScrollbarAll(sbar,8);

  DrawButtonAll(butt, 11);

  signed char aaa;

  while(1){
    //ボタン判定
    pushc = ButtonTouch(butt, 11);
    if(pushc!=-1){
      if(pushc==1)sbar[0].value=0x7fff;
      if(pushc==2)sbar[2].value=0x7fff;
      if(pushc==3)sbar[4].value=0x7fff;
      if(pushc==4)sbar[6].value=0x7fff;
      if(pushc==5){
        sbar[3].value=sbar[1].value;
        sbar[5].value=sbar[1].value;
        sbar[7].value=sbar[1].value;
      }
      if(pushc==6){
        sbar[1].value=sbar[3].value;
        sbar[5].value=sbar[3].value;
        sbar[7].value=sbar[3].value;
      }
      if(pushc==7){
        sbar[1].value=sbar[5].value;
        sbar[3].value=sbar[5].value;
        sbar[7].value=sbar[5].value;
      }
      if(pushc==8){
        sbar[1].value=sbar[7].value;
        sbar[3].value=sbar[7].value;
        sbar[5].value=sbar[7].value;
      }
      DrawScrollbarAll(sbar,8);
    }

    aaa=ScrollbarTouch(sbar,8);
    
    power_par[0]=sbar[1].value>>1;
    power_par[1]=sbar[3].value>>1;
    power_par[2]=sbar[5].value>>1;
    power_par[3]=sbar[7].value>>1;

    tft.fillRect(100,6,32,16,0xf79e);
    tft.fillRect(100,76,32,16,0xf79e);
    tft.fillRect(100,146,32,16,0xf79e);
    tft.fillRect(100,216,32,16,0xf79e);

    sprintf(bmc,"%03d",power_par[0]*100/0x7fff);
    DrawUCfont(100,6,ST77XX_BLUE,bmc);
    sprintf(bmc,"%03d",power_par[1]*100/0x7fff);
    DrawUCfont(100,76,ST77XX_BLUE,bmc);
    sprintf(bmc,"%03d",power_par[2]*100/0x7fff);
    DrawUCfont(100,146,ST77XX_BLUE,bmc);
    sprintf(bmc,"%03d",power_par[3]*100/0x7fff);
    DrawUCfont(100,216,ST77XX_BLUE,bmc);
    
    tft.fillRect(4,6,48,16,0xf79e);
    tft.fillRect(4,76,48,16,0xf79e);
    tft.fillRect(4,146,48,16,0xf79e);
    tft.fillRect(4,216,48,16,0xf79e);
    
    mm1=(sbar[0].value-0x7fff)*power_par[0]/0x7fff;
    mm2=(sbar[2].value-0x7fff)*power_par[1]/0x7fff;
    mm3=(sbar[4].value-0x7fff)*power_par[2]/0x7fff;
    mm4=(sbar[6].value-0x7fff)*power_par[3]/0x7fff;
    sprintf(bmc,"%05d",mm1);
    DrawUCfont(4,6,ST77XX_BLUE,bmc);
    sprintf(bmc,"%05d",mm2);
    DrawUCfont(4,76,ST77XX_BLUE,bmc);
    sprintf(bmc,"%05d",mm3);
    DrawUCfont(4,146,ST77XX_BLUE,bmc);
    sprintf(bmc,"%05d",mm4);
    DrawUCfont(4,216,ST77XX_BLUE,bmc);
    
    motor_move(2,mm1,mm2,mm3,mm4);
    if(pushc==0)Menu();
    delay(16);
  }
  return;
}

void ERROR()//エラー画面
{
  tft.fillScreen(ST77XX_RED);
  tft.setTextSize(2);
  tft.setCursor(32, 100);
  tft.print("Safety Program");
  while (1)
    ;
}

void TP_Adjust(){//タッチパネル調整画面
  CreateButton(&butt[0], 0, 290, 50, 30, 1, "Back");
  tft.fillScreen(0);
  
  int tpx,tpy;

  TS_Point tpp;

  tft.setTextColor(0xffff);


  tft.drawLine(0,0,0,319,ST77XX_RED);

  while(!ts.touched())delay(16);

  while(ts.touched()){
    tpp=ts.getPoint();
    delay(16);
  }
  mintx=tpp.x;
  tft.setCursor(5,156);
  tft.print(mintx);

  tft.drawLine(0,319,239,319,ST77XX_RED);

  while(!ts.touched())delay(16);

  while(ts.touched()){
    tpp=ts.getPoint();
    delay(16);
  }
  maxty=tpp.y;
  tft.setCursor(108,306);
  tft.print(maxty);

  tft.drawLine(239,0,239,319,ST77XX_RED);

  while(!ts.touched())delay(16);

  while(ts.touched()){
    tpp=ts.getPoint();
    delay(16);
  }
  maxtx=tpp.x;
  tft.setCursor(212,156);
  tft.print(maxtx);

  tft.drawLine(0,0,239,0,ST77XX_RED);

  while(!ts.touched())delay(16);

  while(ts.touched()){
    tpp=ts.getPoint();
    delay(16);
  }
  minty=tpp.y;
  tft.setCursor(108,5);
  tft.print(minty);

  DrawButtonAll(butt, 1);
  
  while(1){
    pushc = ButtonTouch(butt, 1);
    if(pushc==0)LCD_Config();

    GetPanelPos(&tpx,&tpy);
    tft.fillCircle(tpx,tpy,3,ST77XX_GREEN);
    delay(16);
  }
  return;
}

void LCD_Config(){//液晶の設定メニュー
  CreateButton(&butt[0], 45, 80, 150, 40, 1, "Touch Panel adjustment");
  CreateButton(&butt[1], 45, 140, 150, 40, 1, "Display Rotation");
  CreateButton(&butt[2], 45, 200, 150, 40, 1, "Drawing BenchMark");
  CreateButton(&butt[3], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 4);

  while(1){
    pushc = ButtonTouch(butt, 4);
    if(pushc==0)TP_Adjust();
    if(pushc==3)Menu();
    delay(16);
  }
  return;
}

void Infomation(){//情報表示

  CreateButton(&butt[0], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(ST77XX_GREEN);

  DrawButtonAll(butt, 1);

  tft.setCursor(70,50);
  tft.setTextColor(0);
  tft.print("Robot Health V1.0");
  tft.setCursor(50,120);
  tft.print("CopyRight 2023 U.Chikara");
  tft.setCursor(30,70);
  tft.setTextColor(ST77XX_RED);
  tft.print("Use processor: ESP32-WROOM-32");
  tft.setCursor(30,80);
  tft.print("Use LCD: ST7789 240*320");
  tft.setCursor(30,90);
  tft.print("Programming: U.Chikara");
  tft.setCursor(30,100);
  tft.print("Design: U.Chikara");


  while(1){
    pushc = ButtonTouch(butt, 1);

    if(pushc==0)Menu();
    delay(16);
  }
  return;
}

void Menu()//メニュー画面
{
  // put your main code here, to run repeatedly:
  CreateButton(&butt[0], 10, 120, 100, 40, 1, "BLess Motor");
  CreateButton(&butt[1], 130, 120, 100, 40, 1, "Brush Motor");
  CreateButton(&butt[2], 0, 180, 80, 40, 1, "Servo");
  CreateButton(&butt[3], 80, 180, 80, 40, 1, "Solenoid");
  CreateButton(&butt[4], 160, 180, 80, 40, 1, "Sensor");
  CreateButton(&butt[5], 10, 240, 100, 40, 1, "Infomation");
  CreateButton(&butt[6], 130, 240, 100, 40, 1, "LCD Config");
  CreateButton(&butt[7], 200, 300, 40, 20, 1, "???");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 8);

  while (1)
  {
    pushc = ButtonTouch(butt, 8);
    Mecanum(AM);
    if(pushc==0)BlessMotor();
    if(pushc==1)Bmotor();
    if(pushc==5)Infomation();
    if(pushc==6)LCD_Config();
    if(pushc==3)SolenoidTest();

    delay(16);
  }
  return;
}

void setup()//初期設定
{
  // put your setup code here, to run once:

  SN.id=5;

  AM.m_id=2;

  moin.m_id=0x205;

  Serial.begin(115200);

  tft.init(240, 320); // Init ST7789 240x240

  tft.invertDisplay(false);

  tft.setSPISpeed(40000000);

  ts.begin();
  ts.setRotation(2);

  // 4C:75:25:92:20:9E
  PS4.begin("9c:9c:1f:cb:d2:c6");

  Solenoid_init();
  can_begin();
  Menu();
}

void loop(){//使わない

}