#include <Arduino.h>
#include <TX16S.h>

uint8_t rxbuf[CRSF_MAX_PACKET_LEN + 3];		// 受信した生データ
uint8_t rxPos = 0;
uint8_t frameSize = 0;
bool datardyf = false;					  	// データが揃った
uint32_t gaptime;					      	// フレーム区切り測定用
uint32_t time_m;				        	// インターバル時間(debug用)
gamepad_data gp;			    		// CH毎に並び替えたデータ


// CRSFから受信した11bitシリアルデータを16bitデータにデコード
void crsfdecode () {
	crsf_frame *crsf =(crsf_frame *)rxbuf;

	if (crsf->device_addr == CRSF_ADDRESS_FLIGHT_CONTROLLER) {	  // ヘッダチェック
		if (crsf->type == CRSF_FRAMETYPE_RC_CHANNELS_PACKED) {	  // CHデータならデコード
			//gp.sw = 0;
			gp.ch[0] = (uint16_t)crsf->data.b11.ch0;
			gp.ch[1] = (uint16_t)crsf->data.b11.ch1;
			gp.ch[2] = (uint16_t)crsf->data.b11.ch2;
			gp.ch[3] = (uint16_t)crsf->data.b11.ch3;
			gp.ch[4] = (uint16_t)crsf->data.b11.ch4;
			gp.ch[5] = (uint16_t)crsf->data.b11.ch5;
			gp.ch[6] = (uint16_t)crsf->data.b11.ch6;
			gp.ch[7] = (uint16_t)crsf->data.b11.ch7;
			gp.ch[8] = (uint16_t)crsf->data.b11.ch8;
			gp.ch[9] = (uint16_t)crsf->data.b11.ch9;
			gp.ch[10] = (uint16_t)crsf->data.b11.ch10;
			gp.ch[11] = (uint16_t)crsf->data.b11.ch11;
			gp.ch[12] = (uint16_t)crsf->data.b11.ch12;
			gp.ch[13] = (uint16_t)crsf->data.b11.ch13;
			gp.ch[14] = (uint16_t)crsf->data.b11.ch14;
			gp.ch[15] = (uint16_t)crsf->data.b11.ch15;

			datardyf = true; // データ揃ったよフラグ
		}
	}
}	

// CRSF受信処理
void crsf(void) {
	uint8_t data;

	// CRSFから1バイト受信
	if (Serial1.available()) {							// Serial1に受信データがあるなら
		data  = Serial1.read();							// 8ビットデータ読込
		gaptime = micros();
		if (rxPos == 1) {
			frameSize = data;							// 2byte目はフレームサイズ
		}
		if(rxPos < CRSF_MAX_PACKET_LEN){
		    rxbuf[rxPos++] = data;						// 受信データをバッファに格納
        }
		if (rxPos > 1 && rxPos == frameSize + 2) {
			crsfdecode();								// １フレーム受信し終わったらデーコードする
			rxPos = 0;
		}
	}
	else {
		if (rxPos > 0 && micros() - gaptime > 300) {	// 300us以上データが来なかったら区切りと判定
			rxPos = 0;
		}
	}
}
