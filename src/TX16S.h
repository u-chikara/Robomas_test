// CrossFireの定義
#include <Arduino.h>

#define CRSF_BAUDRATE 400000
#define CRSF_MAX_PACKET_LEN 64

typedef enum
{
	CRSF_ADDRESS_BROADCAST          = 0x00,
	CRSF_ADDRESS_USB                = 0x10,
	CRSF_ADDRESS_TBS_CORE_PNP_PRO   = 0x80,
	CRSF_ADDRESS_RESERVED1          = 0x8A,
	CRSF_ADDRESS_CURRENT_SENSOR     = 0xC0,
	CRSF_ADDRESS_GPS                = 0xC2,
	CRSF_ADDRESS_TBS_BLACKBOX       = 0xC4,
	CRSF_ADDRESS_FLIGHT_CONTROLLER  = 0xC8, //ELRS受信機からのデータ
	CRSF_ADDRESS_RESERVED2          = 0xCA,
	CRSF_ADDRESS_RACE_TAG           = 0xCC,
	CRSF_ADDRESS_RADIO_TRANSMITTER  = 0xEA,
	CRSF_ADDRESS_CRSF_RECEIVER      = 0xEC,
	CRSF_ADDRESS_CRSF_TRANSMITTER   = 0xEE,
} crsf_addr_e;

typedef enum
{
	CRSF_FRAMETYPE_GPS                = 0x02,
	CRSF_FRAMETYPE_BATTERY_SENSOR     = 0x08,
	CRSF_FRAMETYPE_LINK_STATISTICS    = 0x14,
	CRSF_FRAMETYPE_OPENTX_SYNC        = 0x10,
	CRSF_FRAMETYPE_RADIO_ID           = 0x3A,
	CRSF_FRAMETYPE_RC_CHANNELS_PACKED = 0x16,	// チャンネルパックフレーム
	CRSF_FRAMETYPE_ATTITUDE           = 0x1E,
	CRSF_FRAMETYPE_FLIGHT_MODE        = 0x21,	// Extended Header Frames, range: 0x28 to 0x96
	CRSF_FRAMETYPE_DEVICE_PING        = 0x28,
	CRSF_FRAMETYPE_DEVICE_INFO        = 0x29,
	CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,
	CRSF_FRAMETYPE_PARAMETER_READ     = 0x2C,
	CRSF_FRAMETYPE_PARAMETER_WRITE    = 0x2D,
	CRSF_FRAMETYPE_COMMAND            = 0x32,	// MSP commands
	CRSF_FRAMETYPE_MSP_REQ            = 0x7A,   // response request using msp sequence as command
	CRSF_FRAMETYPE_MSP_RESP           = 0x7B,   // reply with 58 byte chunked binary
	CRSF_FRAMETYPE_MSP_WRITE          = 0x7C,   // write with 8 byte chunked binary (OpenTX outbound telemetry buffer limit)
} crsf_frame_type_e;

#pragma pack(push,1)   	// データを１バイト単位に詰めて配置
//----------- #pragma ここから ------------------------
typedef struct{
    unsigned ch0 : 11;	// 11bit チャンネルデータ
    unsigned ch1 : 11;
    unsigned ch2 : 11;
    unsigned ch3 : 11;
    unsigned ch4 : 11;
    unsigned ch5 : 11;
    unsigned ch6 : 11;
    unsigned ch7 : 11;
    unsigned ch8 : 11;
    unsigned ch9 : 11;
    unsigned ch10 : 11;
    unsigned ch11 : 11;
    unsigned ch12 : 11;
    unsigned ch13 : 11;
    unsigned ch14 : 11;
    unsigned ch15 : 11;
} crsf_channels;

typedef union{			  	// チャンネルデータ共用体
	uint8_t byte[22];
	crsf_channels b11;
} crsf_data;

typedef struct{			  	// CRSFフレーム
	uint8_t device_addr;	// アドレス
	uint8_t frame_size;		// この後からのバイト数
	uint8_t type;		    // タイプ
	crsf_data data;		    // チャンネルデータ
	uint8_t crc;		    // CRC
} crsf_frame;


typedef struct {		  	// ゲームパッドデータ
	uint16_t ch[16];			// 16bit 8ch
} gamepad_data;



//------------#pragma ここまで-------------------------
#pragma pack(pop)

void crsfdecode ();
void crsf(void);

extern uint8_t rxbuf[CRSF_MAX_PACKET_LEN + 3];      // 受信した生データ
extern uint8_t rxPos;
extern uint8_t frameSize;
extern bool datardyf;					  	// データが揃った
extern uint32_t gaptime;					// フレーム区切り測定用
extern uint32_t time_m;				    	// インターバル時間(debug用)
extern gamepad_data gp;			    		// CH毎に並び替えたデータ