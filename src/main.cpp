//Copy Right 2023 U.Chikara

#include <PS4Controller.h>
#include <Robot_Motor.h>
#include <Robot_Solenoid.h>
#include <Yushi_Library.h>
#include <Can_Servo.h>
#include <TX16S.h>

void Menu();
void BlessMotor();
void LCD_Config();

typedef struct
{ // センサー基盤構造体
  uint16_t s_id1 = 12;
  uint16_t s_id2 = 13;
  uint64_t timestamp = 0;
  uint16_t rotaen0 = 0;
  uint16_t rotaen1 = 0;
  uint16_t rotaen2 = 0;
  uint16_t rotaen3 = 0;
  uint16_t rotaen4 = 0;
  uint8_t rimit0 = 0;
  uint8_t rimit1 = 0;
  uint8_t rimit2 = 0;
  uint8_t rimit3 = 0;
  uint8_t rimit4 = 0;
} sensorkiban;

savokiban saki;

sensorkiban seki;

Solenoid SN;

Button_t butt[20];

Scrollbar_t sbar[10];

ashimawari AM;

motor_info moin;

char pushc;

signed short mm1,mm2,mm3,mm4;

char buf[16];

float Kp=1.0;//比例
float Ki=0.42;//積分
float Kd=0.00;//微分

signed int e;//誤差
signed int olde;//以前の誤差
signed int de;//微分
signed int in;//積分
float T=0.001;//時間(ms)


signed int PID_control(signed int inrpm,signed int gotrpm){

  e=inrpm-gotrpm;
  de=(e-olde)/T;
  in=in+(e+olde)*T/2;
  olde=e;
  //return e*Kp+de*Kd+in*Ki;
  return e*Kp+in*Ki+de*Kd;
}
void Mecanum(){//メカナム処理

  // if(PS4.Circle()){
  //   if(PS4.Down())Kp-=0.005;
  //   if(PS4.Up())Kp+=0.05;
  // }
  // if(PS4.Cross()){
  //   if(PS4.Down())Ki-=0.005;
  //   if(PS4.Up())Ki+=0.005;
  // }
  // if(PS4.Square()){
  //   if(PS4.Down())Kd-=0.005;
  //   if(PS4.Up())Kd+=0.005;
  // }
  if(PS4.LStickX()>10 or PS4.LStickX()<-10 or PS4.LStickY()>10 or PS4.LStickY()<-10){
    AM.Rorad=atan2(PS4.LStickY(),PS4.LStickX());
    AM.motor[0]=sin(AM.Rorad+PI/4+AM.offrot)*power_par[0];
    AM.motor[1]=sin(AM.Rorad+PI*3/4+AM.offrot)*power_par[1];
    AM.motor[2]=sin(AM.Rorad+PI*5/4+AM.offrot)*power_par[2];
    AM.motor[3]=sin(AM.Rorad+PI*7/4+AM.offrot)*power_par[3];
  }else{
    AM.motor[0]=0;
    AM.motor[1]=0;
    AM.motor[2]=0;
    AM.motor[3]=0;
  }
  BLmotor_move(Motor_14,PID_control(AM.motor[0],moin.rot_speed[0]),
                        PID_control(AM.motor[1],moin.rot_speed[1]),
                        PID_control(AM.motor[2],moin.rot_speed[2]),
                        PID_control(AM.motor[3],moin.rot_speed[3])
  );
  
  
  return;
}
void RobotProcess(){
  if (PS4.isConnected()){

    Mecanum();
  }
  return;
}


void SensorTest(){
  tft.fillScreen(0xf79e);
  CreateButton(&butt[0], 190, 290, 50, 30, 1, "Back");
  DrawButtonAll(butt, 1);
  while(1){
    pushc = ButtonTouch(butt, 1);
    if(pushc==0)Menu();
  }
  return;
}

void ServoTest(){
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

void can_begin(){//CAN初期化
    CAN.setPins(17, 21); //( rx , tx )
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
    
    if(aaa==1){
      power_par[0]=sbar[1].value>>1;
      tft.fillRect(100,6,32,16,0xf79e);
      sprintf(bmc,"%03d",power_par[0]*100/0x7fff);
      DrawUCfont(100,6,ST77XX_BLUE,bmc);
    }
    if(aaa==3){
      power_par[1]=sbar[3].value>>1;
      tft.fillRect(100,76,32,16,0xf79e);
      sprintf(bmc,"%03d",power_par[1]*100/0x7fff);
      DrawUCfont(100,76,ST77XX_BLUE,bmc);
    }
    if(aaa==5){
      power_par[2]=sbar[5].value>>1;
      tft.fillRect(100,146,32,16,0xf79e);
      sprintf(bmc,"%03d",power_par[2]*100/0x7fff);
      DrawUCfont(100,146,ST77XX_BLUE,bmc);
    }
    if(aaa==7){
      power_par[3]=sbar[7].value>>1;
      tft.fillRect(100,216,32,16,0xf79e);
      sprintf(bmc,"%03d",power_par[3]*100/0x7fff);
      DrawUCfont(100,216,ST77XX_BLUE,bmc);
    }
    if(aaa==0){ 
      mm1=(sbar[0].value-0x7fff)*power_par[0]/0x7fff;
      tft.fillRect(4,6,48,16,0xf79e);
      sprintf(bmc,"%05d",mm1);
      DrawUCfont(4,6,ST77XX_BLUE,bmc);
    }
    if(aaa==2){
      mm2=(sbar[2].value-0x7fff)*power_par[1]/0x7fff;
      tft.fillRect(4,76,48,16,0xf79e);
      sprintf(bmc,"%05d",mm2);
      DrawUCfont(4,76,ST77XX_BLUE,bmc);
    }
    if(aaa==4){
      mm3=(sbar[4].value-0x7fff)*power_par[2]/0x7fff;
      tft.fillRect(4,146,48,16,0xf79e);
      sprintf(bmc,"%05d",mm3);
      DrawUCfont(4,146,ST77XX_BLUE,bmc);
    }
    if(aaa==6){
      mm4=(sbar[6].value-0x7fff)*power_par[3]/0x7fff;
      tft.fillRect(4,216,48,16,0xf79e);
      sprintf(bmc,"%05d",mm4);
      DrawUCfont(4,216,ST77XX_BLUE,bmc);
    }
    
    motor_move(2,mm1,mm2,mm3,mm4);
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
    if(pushc==0)BlessMotorTest();
    if(pushc==1)BmotorTest();
    if(pushc==5)Infomation();
    if(pushc==6)LCD_Config();
    if(pushc==3)SolenoidTest();
    if(pushc==4)SensorTest();
    if(pushc==2)ServoTest();
    RobotProcess();

    delay(1);
  }
  return;
}

TaskHandle_t args[1];

void Core0a(void *args) {
  while (1) {
    crsf();
    if(datardyf){
      int rpi;
      //Serial.print(rxbuf[0], HEX);  		// device addr
      //Serial.print(" ");
      //Serial.print(rxbuf[1]);			 	// data size +1
      //Serial.print(" ");
      //Serial.print(rxbuf[2], HEX);  		// type
      //Serial.print(" ");
      for (rpi = 0; rpi < 16; rpi++) {
        Serial.print(gp.ch[rpi]);
        Serial.print(" ");
      }
      
      Serial.print(" ");
      Serial.print(micros() - time_m); 	// インターバル時間(us)を表示
      Serial.println("us");
      time_m = micros();

      datardyf = false;				// データ揃ったよフラグをクリア
    }
  }
}

void setup()//初期設定
{
  // put your setup code here, to run once:

  xTaskCreatePinnedToCore(Core0a, "Core0a", 4096, NULL, 2, &args[0], 0); 

  saki.v_id=144;//サーボ基盤のID

  SN.id=0x0f1;//ソレノイド基盤のID

  Serial.begin(115200);
  while(!Serial);

  datardyf = false;
	gaptime = 0;
	rxPos = 0;
	//Serial.begin(CRSF_BAUDRATE);								// PCシリアル通信用 (受信機の速度に合わせる)
  //                                       RX  TX
	Serial1.begin(CRSF_BAUDRATE, SERIAL_8N1, 22, 23);	// CRSF通信用 (420kbps, 8bitdata, nonParity, 1stopbit)
  while(!Serial1);
	time_m = micros();											// インターバル測定用

  Serial1.setRxBufferSize(1024);
  delay(10);


  tft.init(240, 320); // Init ST7789 320x240

  tft.invertDisplay(false);

  tft.setSPISpeed(20000000);

  ts.begin();
  ts.setRotation(2);

  delay(10);

  //PS4.begin("4C:75:25:92:20:9E");//白色PS4こん
  PS4.begin("00:1b:dc:0f:bc:40");//俺の赤いPS4こん

  can_begin();

  Cylinder_offtime(1000,1000,1000,1000);

  Menu();
}

void loop(){//使わない でも消すんじゃねぇ エラるから

}