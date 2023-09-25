//Copy Right 2023 U.Chikara

/*
  0 11 001000
 10 00 001000
 11 01 001000
100 10 001000
101 11 001000
111 00 001000

0
2
3
4
5
7

*/

#define CAN_Enable 1

#define T_Center 1
#define T_Up 3
#define T_Down 0

#include <PS4Controller.h>
#include <Robot_Motor.h>
#include <Robot_Solenoid.h>
#include <Yushi_Library.h>
#include <Can_Servo.h>
#include <TX16S.h>
#include <Sensor.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define SM 0.707106781


void Menu();
void BlessMotor();
void LCD_Config();

savokiban saki;

sensorkiban seki;

Solenoid SN;

Button_t butt[20];

Scrollbar_t sbar[10];

ashimawari AM;

motor_info moin;
motor_info moin_sub;

volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

char pushc;

char buf[16];//文字バッファ

float Kp=1.0;//比例
float Ki=0.42;//積分
float Kd=0.001;//微分

signed int e;//誤差
signed int olde;//以前の誤差
signed int de;//微分
signed int in;//積分
float T=0.012;//時間(ms)

typedef struct {//TX16S入力構造体
  signed short uxLX;
  signed short uxLY;
  signed short uxRX;
  signed short uxRY;
  signed char uxSF;
  signed char uxSE;
  signed char uxSA;
  signed char uxSB;
  signed char uxSC;
  signed char uxSD;
  signed char uxSG;
  signed char uxSH;
  signed short uxLS;
  signed short uxRS;
  signed char uxS1;
  signed char uxS2;
}TX16S_Con;

TX16S_Con TC;

//  3 1800
//  1 100
//  0 200
unsigned char sf=0,sh=0;
signed char sa=0,sb=0,se=1,sg=0;
signed char saB=0;
unsigned short rs=200;
unsigned char sasu=0;

boolean flag=0;

signed short danmotor=0;

boolean saBB;

unsigned char square=0;

void can_rec(int packetSize)//CAN割り込み受信
{ // センサーデーター受信
  if (CAN.packetId() == 0x201)
  {
    moin.mech_angle[0] = CAN.read() << 8 | CAN.read();
    moin.rot_speed[0] = CAN.read() << 8 | CAN.read();
    moin.current[0] = CAN.read() << 8 | CAN.read();
    moin.temp[0] = CAN.read();
  }
  if (CAN.packetId() == 0x202)
  {
    moin.mech_angle[1] = CAN.read() << 8 | CAN.read();
    moin.rot_speed[1] = CAN.read() << 8 | CAN.read();
    moin.current[1] = CAN.read() << 8 | CAN.read();
    moin.temp[1] = CAN.read();
  }
  if (CAN.packetId() == 0x203)
  {
    moin.mech_angle[2] = CAN.read() << 8 | CAN.read();
    moin.rot_speed[2] = CAN.read() << 8 | CAN.read();
    moin.current[2] = CAN.read() << 8 | CAN.read();
    moin.temp[2] = CAN.read();
  }
  if (CAN.packetId() == 0x204)
  {
    moin.mech_angle[3] = CAN.read() << 8 | CAN.read();
    moin.rot_speed[3] = CAN.read() << 8 | CAN.read();
    moin.current[3] = CAN.read() << 8 | CAN.read();
    moin.temp[3] = CAN.read();
  }
  if (CAN.packetId() == 0x205)
  {
    moin_sub.mech_angle[0] = CAN.read() << 8 | CAN.read();
    moin_sub.rot_speed[0] = CAN.read() << 8 | CAN.read();
    moin_sub.current[0] = CAN.read() << 8 | CAN.read();
    moin_sub.temp[0] = CAN.read();
  }
  if (CAN.packetId() == 0x206)
  {
    moin_sub.mech_angle[1] = CAN.read() << 8 | CAN.read();
    moin_sub.rot_speed[1] = CAN.read() << 8 | CAN.read();
    moin_sub.current[1] = CAN.read() << 8 | CAN.read();
    moin_sub.temp[1] = CAN.read();
  }
  if (CAN.packetId() == 0x207)
  {
    moin_sub.mech_angle[2] = CAN.read() << 8 | CAN.read();
    moin_sub.rot_speed[2] = CAN.read() << 8 | CAN.read();
    moin_sub.current[2] = CAN.read() << 8 | CAN.read();
    moin_sub.temp[2] = CAN.read();
  }
  if (CAN.packetId() == 0x208)
  {
    moin_sub.mech_angle[3] = CAN.read() << 8 | CAN.read();
    moin_sub.rot_speed[3] = CAN.read() << 8 | CAN.read();
    moin_sub.current[3] = CAN.read() << 8 | CAN.read();
    moin_sub.temp[3] = CAN.read();
  }

  unsigned char _rim0, _rim1;
  if (CAN.packetId() == seki.s_id1)
  {
    seki.timestamp = CAN.read() | (CAN.read() << 8) | (CAN.read() << 16) | (CAN.read() << 24) | (CAN.read() << 32);
    _rim0 = CAN.read();
    seki.rotaen0 = CAN.read() << 8 | CAN.read();

    seki.rimit0 = _rim0 & 1;
    seki.rimit1 = (_rim0 >> 1) & 1;
    seki.rimit2 = (_rim0 >> 2) & 1;
    seki.rimit3 = (_rim0 >> 3) & 1;
    seki.rimit4 = (_rim0 >> 4) & 1;
  }

  if (CAN.packetId() == seki.s_id2)
  {
    seki.rotaen1 = CAN.read() << 8 | CAN.read();
    seki.rotaen2 = CAN.read() << 8 | CAN.read();
    seki.rotaen3 = CAN.read() << 8 | CAN.read();
    seki.rotaen4 = CAN.read() << 8 | CAN.read();
  }
}

signed int PID_control(signed int inrpm,signed int gotrpm){//PID処理
  e=inrpm-gotrpm;
  de=(e-olde)/T;
  in=in+(e+olde)*T/2;
  olde=e;
  return e*Kp+in*Ki+de*Kd;
}

void Mecanum(){//メカナム処理
  if(TC.uxRX>50 or TC.uxRX<-50 or TC.uxRY>50 or TC.uxRY<-50 or TC.uxLX>50 or TC.uxLX<-50){
    //TC.uxRX=-TC.uxRX;
    TC.uxRY=-TC.uxRY;
    AM.motor[0]=(TC.uxRX*SM+TC.uxRY*SM)*10+TC.uxLX*6;
    AM.motor[1]=(TC.uxRX*(-SM)+TC.uxRY*SM)*10+TC.uxLX*6;
    AM.motor[2]=(TC.uxRX*(-SM)-TC.uxRY*SM)*10+TC.uxLX*6;
    AM.motor[3]=(TC.uxRX*SM-TC.uxRY*SM)*10+TC.uxLX*6;

    //printf("%d,%d <%d,%d,%d,%d>\n",TC.uxLX,TC.uxLY,AM.motor[0],AM.motor[1],AM.motor[2],AM.motor[3]);
  }else{
    AM.motor[0]=0;
    AM.motor[1]=0;
    AM.motor[2]=0;
    AM.motor[3]=0;
  }
  #if CAN_Enable
  BLmotor_move(Motor_14,PID_control(AM.motor[0],moin.rot_speed[0]),
                        PID_control(AM.motor[1],moin.rot_speed[1]),
                        PID_control(AM.motor[2],moin.rot_speed[2]),
                        PID_control(AM.motor[3],moin.rot_speed[3])
  );
  #endif
  
  
  return;
}

void RobotProcess(){//ロボットメイン処理
  #if CAN_Enable
    if(TC.uxSF^sf){//段越えシリンダの上下
      sf=TC.uxSF;
      if(TC.uxSF==T_Down){
        
        Cylinder_move(1,1,0,0);
        flag=0;
      }
      if(TC.uxSF==T_Up){
        Cylinder_move(2,2,0,0);
        flag=1;
      }
    }

    if(TC.uxSE^se){//段越えシリンダの片側
      se=TC.uxSE;
      //if(TC.uxSF==T_Up or flag==1){
      if(TC.uxSE==T_Down)Cylinder_move(1,2,0,0);//後輪上がる
      if(TC.uxSE==T_Up)Cylinder_move(2,1,0,0);//前輪上がる
      if(TC.uxSE==T_Center)Cylinder_move(2,2,0,0);
      //}
    }

    if(TC.uxSH^sh){//補助輪両足
      sh=TC.uxSH;
      Cylinder_offtime(1000,1000,1000,1000);
      if(TC.uxSH==T_Down)Cylinder_move(0,0,1,0);
      if(TC.uxSH==T_Up)Cylinder_move(0,0,2,0);
    }

    if(TC.uxRS^rs){//補助輪が砂州になった！
      
      if(rs<TC.uxRS)sasu=1;
      if(rs>TC.uxRS)sasu=0;
      if(rs==1800 and TC.uxRS==200)sasu=1;
      if(rs==200 and TC.uxRS==1800)sasu=0;

      if(TC.uxSD==T_Down)Cylinder_offtime(100,1000,1000,1000);
      if(TC.uxSD==T_Center)Cylinder_offtime(1000,1000,100,1000);
      if(TC.uxSD==T_Up)Cylinder_offtime(1000,100,1000,1000);
      
      if(sasu==1){
        if(TC.uxSD==T_Down)Cylinder_move(2,0,0,0);
        if(TC.uxSD==T_Center)Cylinder_move(0,0,2,0);
        if(TC.uxSD==T_Up)Cylinder_move(0,2,0,0);
      }else{
        if(TC.uxSD==T_Down)Cylinder_move(1,0,0,0);
        if(TC.uxSD==T_Center)Cylinder_move(0,0,1,0);
        if(TC.uxSD==T_Up)Cylinder_move(0,1,0,0);
      }
      rs=TC.uxRS;
    }
    if(PS4.Circle()){
      Cylinder_offtime(1000,1000,100,1000);
      Cylinder_move(0,0,2,0);
    }
    if(PS4.Cross()){
      Cylinder_offtime(1000,1000,100,1000);
      Cylinder_move(0,0,1,0);
    }

    
    //お助けアイテムモーター
    if(TC.uxSA==T_Center){
      sa=0;
    }
    if(TC.uxSA==T_Down){
      sa=-1;
      if(saB==-1)sa=0;
    }
    if(TC.uxSA==T_Up){
      sa=1;
      if(saB==1)sa=0;
    }
    if(seki.rimit0^saBB){
      saBB=seki.rimit0;
      if(saBB)saB=sa;
    }

    //お助けアイテムモーター２
    if(TC.uxSG==T_Center)sg=0;
    if(TC.uxSG==T_Down)sg=-1;
    if(TC.uxSG==T_Up)sg=1;
    
    //ロジャー上下
    if(TC.uxSB==T_Center)sb=0;
    if(TC.uxSB==T_Down)sb=-1;
    if(TC.uxSB==T_Up)sb=1;

    

    if(TC.uxRY>50 or TC.uxRY<-50){
      danmotor=TC.uxRY*17;
    }else{
      danmotor=0;
    }

    motor_move(3,sa*9000,sb*20000,sg*10000,0);//お助け ロジャー
    Mecanum();
    BLmotor_move(Motor_58,PID_control(danmotor,moin_sub.rot_speed[0]),
                        PID_control(-danmotor,moin_sub.rot_speed[1]),
                        0,
                        0
    );

    //printf("%d,%d,%d\n",danmotor,moin_sub.rot_speed[0],moin_sub.rot_speed[1]);

    //printf("SF=%d,SH=%d,SA=%d,SB=%d\n",TC.uxSF,TC.uxSH,TC.uxSA,TC.uxSB);
    #endif
  return;
}

void SensorTest(){//センサー基盤
  tft.fillScreen(0xf79e);
  CreateButton(&butt[0], 190, 290, 50, 30, 1, "Back");
  DrawButtonAll(butt, 1);
  while(1){
    pushc = ButtonTouch(butt, 1);
    tft.fillCircle(60,10,10,ST77XX_RED*seki.rimit0);//
    tft.fillCircle(60,30,10,ST77XX_RED*seki.rimit1);//
    tft.fillCircle(60,50,10,ST77XX_RED*seki.rimit2);//
    tft.fillCircle(60,70,10,ST77XX_RED*seki.rimit3);//
    tft.fillCircle(60,90,10,ST77XX_RED*seki.rimit4);//
    delay(16);
    if(pushc==0)Menu();
  }
  return;
}

void ServoTest(){//サーボ基盤
  tft.fillScreen(0xf79e);
  CreateButton(&butt[0], 190, 290, 50, 30, 1, "Back");
  CreateButton(&butt[1], 0, 0, 50, 30, 1, "0%");
  CreateButton(&butt[2], 50, 0, 50, 30, 1, "50%");
  CreateButton(&butt[3], 100, 0, 50, 30, 1, "100%");
  
  DrawButtonAll(butt, 4);
  while(1){
    pushc = ButtonTouch(butt, 4);
    if(pushc==0)Menu();

    if(pushc==1)saki.servo2=0;
    if(pushc==2)saki.servo2=127;
    if(pushc==3)saki.servo2=255;
      
    
    voba_move(saki);
  }
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
  for(spb=0;spb<4;spb++){
  CreateButton(&butt[9+spb*2],90,0+spb*80,50,40,1,"Pis_Ext");
  CreateButton(&butt[10+spb*2],90,40+spb*80,50,40,1,"Pis_Shr");
  }
  CreateButton(&butt[17], 170, 20, 50, 30, 1, "Rete");

  DrawButtonAll(butt, 18);

  boolean switches[8]={0};

  boolean rete=0;

  while(1){
    pushc = ButtonTouch(butt, 18);
    if(pushc==0)Menu();
    if(pushc>0){
      if(pushc<9){
        if(pushc==1)switches[0]=!switches[0];
        if(pushc==2)switches[1]=!switches[1];
        if(pushc==3)switches[2]=!switches[2];
        if(pushc==4)switches[3]=!switches[3];
        if(pushc==5)switches[4]=!switches[4];
        if(pushc==6)switches[5]=!switches[5];
        if(pushc==7)switches[6]=!switches[6];
        if(pushc==8)switches[7]=!switches[7];
        if(rete==0)Solenoid_move(switches[0]+1,switches[1]+1,switches[2]+1,switches[3]+1,switches[4]+1,switches[5]+1,switches[6]+1,switches[7]+1);
        for(spb=0;spb<8;spb++){
          tft.fillCircle(60,20+spb*40,7,ST77XX_BLUE+0xf7e1*switches[spb]);
        }
      }else{
        if(pushc==9)Cylinder_move(1,0,0,0);
        if(pushc==10)Cylinder_move(2,0,0,0);
        if(pushc==11)Cylinder_move(0,1,0,0);
        if(pushc==12)Cylinder_move(0,2,0,0);
        if(pushc==13)Cylinder_move(0,0,1,0);
        if(pushc==14)Cylinder_move(0,0,2,0);
        if(pushc==15)Cylinder_move(0,0,0,1);
        if(pushc==16)Cylinder_move(0,0,0,2);
      }
     }
    if(pushc==17){
      rete=!rete;
      if(rete==1){
        tft.drawRoundRect(168,18,54,34,3,ST77XX_RED);
      }else{
        tft.drawRoundRect(168,18,54,34,3,0xf79e);
        Solenoid_move(switches[0]+1,switches[1]+1,switches[2]+1,switches[3]+1,switches[4]+1,switches[5]+1,switches[6]+1,switches[7]+1);
      }
    }
    delay(16);
  }
  
  Menu();
  return;
}

void can_begin(){//CAN初期化
    CAN.setPins(21, 17); //( rx , tx )
    CAN.begin(1E6);
    CAN.onReceive(can_rec);
    return;
}

void BlessMotorTest()//ブラシレスモーター項目画面
{
  CreateButton(&butt[0], 0, 210, 30, 30, 1, "MT_1");
  CreateButton(&butt[1], 30, 210, 30, 30, 1, "MT_2");
  CreateButton(&butt[2], 60, 210, 30, 30, 1, "MT_3");
  CreateButton(&butt[3], 90, 210, 30, 30, 1, "MT_4");
  CreateButton(&butt[4], 120, 210, 30, 30, 1, "MT_5");
  CreateButton(&butt[5], 150, 210, 30, 30, 1, "MT_6");
  CreateButton(&butt[6], 180, 210, 30, 30, 1, "MT_7");
  CreateButton(&butt[7], 210, 210, 30, 30, 1, "MT_8");
  CreateButton(&butt[8], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 9);

  unsigned char grx;

  unsigned char bm_id=0;

  while(1){
    //青線表示
    tft.drawLine(0, 119, 239, 119, ST77XX_BLUE);
    tft.drawLine(119, 0, 119, 119, ST77XX_BLUE);

    // RPMグラフ表示
    tft.drawLine(grx, 0, grx, 118, ST77XX_BLACK);
    tft.drawPixel(grx, 63 - moin.rot_speed[bm_id] / 153, ST77XX_GREEN);
    tft.drawPixel(grx, 63 - AM.motor[bm_id] / 153, ST77XX_CYAN);
    if(grx&1)tft.drawPixel(grx,32,ST77XX_BLUE);
    if(grx&1)tft.drawPixel(grx,92,ST77XX_BLUE);

    // 電流グラフ表示
    tft.drawLine(grx + 120, 0, grx + 120, 118, ST77XX_BLACK);
    tft.drawPixel(grx + 120, 63 - moin.current[bm_id] / 157, ST77XX_YELLOW);

    grx++;
    if (grx > 118)
      grx = 0;

    // RPM表示
    tft.fillRect(0, 0, 48, 14, 0);
    sprintf(buf, "%04d", moin.rot_speed);
    DrawUCfont(0, 0, ST77XX_RED, buf);

    // 電流表示
    tft.fillRect(120, 0, 48, 14, 0);
    sprintf(buf, "%d", moin.current);
    DrawUCfont(120, 0, ST77XX_RED, buf);

    // motor値
    tft.fillRect(0, 120, 48, 14, 0xf79e);
    sprintf(buf, "%.2f", Kp);
    DrawUCfont(0, 120, ST77XX_RED, buf);

    // rpm値
    tft.fillRect(0, 136, 48, 14, 0xf79e);
    sprintf(buf, "%.2f", Ki);
    DrawUCfont(0, 136, ST77XX_RED, buf);

    //tyousei値
    tft.fillRect(0, 152 , 80, 14, 0xf79e);
    sprintf(buf, "%.2f", Kd);
    DrawUCfont(0, 152, ST77XX_RED, buf);

    //printf("rot_speed=%d,Kp=%f,Ki=%f,Kd=%f\r\n",moin.rot_speed,Kp,Ki,Kd);

    RobotProcess();

    pushc = ButtonTouch(butt, 9);

    if(pushc>=0 and pushc<8)bm_id=pushc;

    if(pushc==8)Menu();

    delay(10);
  }

  return;
}

void BmotorTest(){//ブラシモーター項目画面
  signed short mm[4]={0};
  unsigned char bmb=0;
  uint16_t bt_id=3;

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

  tft.setCursor(65,285);
  tft.printf("canid=%d",bt_id);

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
      if(pushc==9 && bt_id>0){
          bt_id--;
          tft.fillRect(75,285,80,8,0xf79e);
          tft.setCursor(65,285);
          tft.printf("canid=%d",bt_id);
      }
      if(pushc==10){
        bt_id++;
          tft.fillRect(75,285,80,8,0xf79e);
          tft.setCursor(65,285);
          tft.printf("canid=%d",bt_id);
      }
    }

    aaa=ScrollbarTouch(sbar,8);

    if(aaa!=-1){
      if(aaa%2==0){
        aaa=aaa>>1;
        mm[aaa]=(sbar[aaa<<1].value-0x7fff)*power_par[aaa]/0x7fff;
        tft.fillRect(4,6+aaa*70,48,16,0xf79e);
        sprintf(bmc,"%05d",mm[aaa]);
        DrawUCfont(4,6+aaa*70,ST77XX_BLUE,bmc);
      }else{
        aaa=(aaa-1)>>1;
        power_par[aaa]=sbar[(aaa<<1)+1].value>>1;
        tft.fillRect(100,6+aaa*70,32,16,0xf79e);
        sprintf(bmc,"%03d",power_par[aaa]*100/0x7fff);
        DrawUCfont(100,6+aaa*70,ST77XX_BLUE,bmc);
      }
    }
    
    
    
    motor_move(bt_id,mm[0],mm[1],mm[2],mm[3]);
    if(pushc==0)Menu();
    delay(16);
  }
  return;
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

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 1);

  tft.setCursor(70,50);
  tft.setTextColor(0);
  tft.print("Robot Health V1.0");
  tft.setCursor(50,120);
  tft.print("CopyRight 2023 U.Chikara");
  tft.setCursor(30,70);
  tft.setTextColor(ST77XX_RED);
  tft.print("Use processor: ESP32-WROOM-32D");
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

void PS4Con(){
  CreateButton(&butt[0], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 1);

  tft.drawCircle(60,60,41,0);
  tft.drawCircle(180,60,41,0);

  unsigned char pclx=0;
  unsigned char pcly=0;
  unsigned char pcrx=0;
  unsigned char pcry=0;


  while(1){
    pushc = ButtonTouch(butt, 1);
    if(pushc==0)Menu();

    if(PS4.isConnected()){

      tft.fillCircle(pclx,pcly,5,0xf79e);
      tft.fillCircle(pcrx,pcry,5,0xf79e);
      pclx=60+PS4.LStickX()/4;
      pcly=60-PS4.LStickY()/4;
      pcrx=180+PS4.RStickX()/4;
      pcry=60-PS4.RStickY()/4;
      tft.fillCircle(pclx,pcly,5,ST77XX_RED*PS4.L3());
      tft.fillCircle(pcrx,pcry,5,ST77XX_RED*PS4.R3());

      tft.drawRoundRect(110,80,8,129,1,0);
      tft.drawRoundRect(122,80,8,128,1,0);
      tft.fillRect(111,81,6,127,0xf79e);
      tft.fillRect(123,81,6,127,0xf79e);
      tft.fillRect(111,81,6,PS4.L2Value()/2,ST77XX_RED);
      tft.fillRect(123,81,6,PS4.R2Value()/2,ST77XX_RED);

      tft.fillCircle(40,160,10,ST77XX_RED*PS4.Left());//left
      tft.fillCircle(80,160,10,ST77XX_RED*PS4.Right());//right
      tft.fillCircle(60,140,10,ST77XX_RED*PS4.Up());//up
      tft.fillCircle(60,180,10,ST77XX_RED*PS4.Down());//down
      tft.fillCircle(160,160,10,ST77XX_RED*PS4.Square());//circle
      tft.fillCircle(200,160,10,ST77XX_RED*PS4.Circle());//cross
      tft.fillCircle(180,140,10,ST77XX_RED*PS4.Triangle());//square
      tft.fillCircle(180,180,10,ST77XX_RED*PS4.Cross());//triangle
      tft.fillCircle(180,140,10,ST77XX_RED*PS4.Triangle());//square
      tft.fillCircle(180,180,10,ST77XX_RED*PS4.Cross());//triangle
      tft.fillCircle(30,240,10,ST77XX_RED*PS4.L1());
      tft.fillCircle(210,240,10,ST77XX_RED*PS4.R1());
      tft.fillCircle(60,240,10,ST77XX_RED*PS4.Share());
      tft.fillCircle(180,240,10,ST77XX_RED*PS4.Options());
      tft.fillCircle(120,230,10,ST77XX_RED*PS4.PSButton());
      tft.fillCircle(120,250,10,ST77XX_RED*PS4.Touchpad());
    }
    delay(16);
  }
  return;
}

void TX16S(){
  CreateButton(&butt[0], 0, 290, 50, 30, 1, "Back");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 1);

  tft.drawRect(6,6,114,114,0);
  tft.drawRect(120,6,114,114,0);

  unsigned char pclx=0;
  unsigned char pcly=0;
  unsigned char pcrx=0;
  unsigned char pcry=0;

  //  3
  //  1
  //  0
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(30,220);
  tft.print("SA");
  tft.setCursor(90,220);
  tft.print("SB");
  tft.setCursor(150,220);
  tft.print("SC");
  tft.setCursor(210,220);
  tft.print("SD");
  tft.setCursor(30,180);
  tft.print("SE");
  tft.setCursor(30,140);
  tft.print("SF");
  tft.setCursor(210,180);
  tft.print("SG");
  tft.setCursor(210,140);
  tft.print("SH");

  uint16_t keycolors[4]={ST77XX_BLACK,ST77XX_BLUE,0,ST77XX_RED};

  while(1){
    pushc = ButtonTouch(butt, 1);
    if(pushc==0)Menu();


    tft.fillCircle(pclx,pcly,5,0xf79e);
    tft.fillCircle(pcrx,pcry,5,0xf79e);
    pclx=63+TC.uxLX/15;
    pcly=63-TC.uxLY/15;
    pcrx=176+TC.uxRX/15;
    pcry=63-TC.uxRY/15;
    tft.fillCircle(pclx,pcly,5,ST77XX_RED);
    tft.fillCircle(pcrx,pcry,5,ST77XX_RED);

    // tft.drawRoundRect(110,80,8,129,1,0);
    // tft.drawRoundRect(122,80,8,128,1,0);
    // tft.fillRect(111,81,6,127,0xf79e);
    // tft.fillRect(123,81,6,127,0xf79e);
    // tft.fillRect(111,81,6,PS4.L2Value()/2,ST77XX_RED);
    // tft.fillRect(123,81,6,PS4.R2Value()/2,ST77XX_RED);

    //F        H
    //E        G
    //A B    C D
    
    tft.fillCircle(30,240,10,keycolors[TC.uxSA]);
    tft.fillCircle(90,240,10,keycolors[TC.uxSB]);
    tft.fillCircle(150,240,10,keycolors[TC.uxSC]);
    tft.fillCircle(210,240,10,keycolors[TC.uxSD]);
    tft.fillCircle(30,200,10,keycolors[TC.uxSE]);
    tft.fillCircle(30,160,10,keycolors[TC.uxSF]);
    tft.fillCircle(210,200,10,keycolors[TC.uxSG]);
    tft.fillCircle(210,160,10,keycolors[TC.uxSH]);
    
    delay(16);
  }
  
}

void IRAM_ATTR Speaker(){
  portENTER_CRITICAL_ISR(&timerMux);
  dacWrite(25,square);
  square=~square;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void Menu()//メニュー画面
{
  // put your main code here, to run repeatedly:
  CreateButton(&butt[0], 10, 120, 100, 40, 1, "BLess Motor");
  CreateButton(&butt[1], 130, 120, 100, 40, 1, "Brush Motor");
  CreateButton(&butt[2], 0, 165, 80, 40, 1, "Servo");
  CreateButton(&butt[3], 80, 165, 80, 40, 1, "Solenoid");
  CreateButton(&butt[4], 160, 165, 80, 40, 1, "Sensor");
  CreateButton(&butt[5], 10, 210, 100, 40, 1, "Infomation");
  CreateButton(&butt[6], 130, 210, 100, 40, 1, "LCD Config");
  CreateButton(&butt[7], 0, 255, 80, 40, 1, "PS4Con");
  CreateButton(&butt[8], 80, 255, 80, 40, 1, "TX16S");
  CreateButton(&butt[9], 160, 255, 80, 40, 1, "???");

  tft.fillScreen(0xf79e);

  DrawButtonAll(butt, 10);

  while (1)
  {
    pushc = ButtonTouch(butt, 10);
    if(pushc==0)BlessMotorTest();
    if(pushc==1)BmotorTest();
    if(pushc==5)Infomation();
    if(pushc==6)LCD_Config();
    if(pushc==3)SolenoidTest();
    if(pushc==4)SensorTest();
    if(pushc==2)ServoTest();
    if(pushc==7)PS4Con();
    if(pushc==8)TX16S();
    RobotProcess();
    delay(10);
  }
  return;
}

void Core0a(void *pvParameters) {//TX16S受信(別タスク)
  while (1) {
    crsf();
    if(datardyf){
      TC.uxLX=(gp.ch[0]-1000);
      TC.uxLY=(gp.ch[1]-1000);
      TC.uxRX=(gp.ch[2]-1000);
      TC.uxRY=(gp.ch[3]-1000);
      TC.uxSF=(gp.ch[4]>>9);
      TC.uxSE=(gp.ch[5]>>9);
      TC.uxSA=(gp.ch[6]>>9);
      TC.uxSB=(gp.ch[7]>>9);
      TC.uxSC=(gp.ch[8]>>9);
      TC.uxSD=(gp.ch[9]>>9);
      TC.uxSG=(gp.ch[10]>>9);
      TC.uxSH=(gp.ch[11]>>9);
      TC.uxLS=(gp.ch[12]-1000);
      TC.uxRS=(gp.ch[13]);
      TC.uxS1=(gp.ch[14]>>9);
      TC.uxS2=(gp.ch[15]>>9);

      datardyf = false;				// データ揃ったよフラグをクリア
    }else{
        if(PS4.isConnected()){
          TC.uxLX=PS4.LStickX()*800/128;
          TC.uxLY=PS4.LStickY()*800/128;
          TC.uxRX=PS4.RStickX()*800/128;
          TC.uxRY=PS4.RStickY()*800/128;

          TC.uxSF=1+PS4.Up()*2+PS4.Down()*(-1);
          TC.uxSE=1+PS4.Left()*2+PS4.Right()*(-1);
          TC.uxSH=1+PS4.L1()*2+PS4.R1()*(-1);
        }
    }
  }
}

void setup()//初期設定
{
  // put your setup code here, to run once:

  xTaskCreatePinnedToCore(Core0a, "Core0a", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE);

  saki.v_id=144;//サーボ基盤のID

  SN.id=0x0f1;//ソレノイド基盤のID

  seki.s_id1=9;//センサー基盤
  seki.s_id2=10;//センサー基盤

  Serial.begin(115200);
  delay(10);
  //while(!Serial);

  datardyf = false;
	gaptime = 0;
	rxPos = 0;
	//Serial.begin(CRSF_BAUDRATE);								// PCシリアル通信用 (受信機の速度に合わせる)
  //                                       RX  TX
	Serial1.begin(CRSF_BAUDRATE, SERIAL_8N1, 22, 23);	// CRSF通信用 (420kbps, 8bitdata, nonParity, 1stopbit)
  //while(!Serial1);
  delay(10);
	time_m = micros();											// インターバル測定用

  //Serial1.setRxBufferSize(1024);
  delay(100);


  tft.init(240, 320); // Init ST7789 320x240

  tft.invertDisplay(false);

  //tft.setSPISpeed(4000000);

  delay(100);

  ts.begin();
  ts.setRotation(2);

  delay(100);

  PS4.begin("4C:75:25:92:20:9E");//白色PS4こん
  //PS4.begin("8c:55:4a:d8:0b:e8");//俺の赤いPS4こん

#if CAN_Enable
  can_begin();

  Cylinder_offtime(1000,1000,1000,1000);

  timer = timerBegin(2, 80, true);
	timerAttachInterrupt(timer, &Speaker, true);

  
	timerAlarmWrite(timer, 500, true);
  timerAlarmEnable(timer);
	delay(50);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
	delay(50);
  timerAlarmDisable(timer);
#endif

  Menu();
}

void loop(){//使わない でも消すんじゃねぇ エラるから

}