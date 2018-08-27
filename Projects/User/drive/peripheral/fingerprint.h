
#ifndef __FINGERPRINT_H
#define __FINGERPRINT_H


#include <stdint.h>
#include "safety.h"

#define  QS808_TIMx               TIM17
#define  QS808_CMD_TIMEOUT        350  //ms

#define  QS808_USARTx             USART1
#define  QS808_DMA_TX             DMA1_Channel2
#define  QS808_DMA_RX             DMA1_Channel3 

#define  CMD_TX_PREFIX            0xAA55
#define  CMD_RX_PREFIX            0x55AA

#define  CMD_TEST_CONNECTION 			0x0001 //进行与设备的通讯测试
#define  CMD_SET_PARAM 						0x0002 // 设置设备参数 (Device ID, Security Level,Baudrate, Duplication Check, Auto Learn)
#define  CMD_GET_PARAM 						0x0003 //获取设备参数 (Device ID, Security Level,Baudrate, Duplication Check, Auto Learn)
#define  CMD_GET_DEVICE_INFO 			0x0004 //获取设备信息
//#define  CMD_ENTER_IAP_MODE				0x0005 //将设备设置为 IAP 状态
#define  CMD_GET_IMAGE 						0x0020 //从采集器采集指纹图像并保存于 ImageBuffer中
#define  CMD_FINGER_DETECT 				0x0021 //检测指纹输入状态
//#define  CMD_UP_IMAGE 						0x0022 //将保存于 ImageBuffer中的指纹图像上传至 HOST
//#define  CMD_DOWN_IMAGE 					0x0023 //HOST 下载指纹图像到模块的 ImageBuffer 中
//#define  CMD_SLED_CTRL 						0x0024 //控制采集器背光灯的开/关（注：半导体传感器不用此功能）
#define  CMD_STORE_CHAR 					0x0040 //将指定编号 Ram Buffer 中的 Template，注册到指定编号的库中
#define  CMD_LOAD_CHAR 						0x0041 //读取库中指定编号中的 Template 到指定编号的Ram Buffer
//#define  CMD_UP_CHAR 							0x0042 //将保存于指定编号的 Ram Buffer 中的Template 上传至 HOST
//#define  CMD_DOWN_CHAR 						0x0043 //从 HOST 下载 Template 到模块指定编号的 RamBuffer 中
#define  CMD_DEL_CHAR 						0x0044 //删除指定编号范围内的 Template 。
#define  CMD_GET_EMPTY_ID 				0x0045 //获取指定范围内可注册的（没有注册的）第一个模板编号。
#define  CMD_GET_STATUS 					0x0046 //获取指定编号的模板注册状态。
#define  CMD_GET_BROKEN_ID 				0x0047 //检查指定编号范围内的所有指纹模板是否存在坏损的情况
#define  CMD_GET_ENROLL_COUNT			0x0048 //获取指定编号范围内已注册的模板个数。
#define  CMD_GENERATE 						0x0060 //将 ImageBuffer 中的指纹图像生成模板数据，并保存于指定编号的 Ram Buffer 中。
#define  CMD_MERGE 								0x0061 //将保存于 Ram Buffer 中的两或三个模板数据融合成一个模板数据
#define  CMD_MATCH 								0x0062 //指定 Ram Buffer 中的两个指纹模板之间进行1:1 比对
#define  CMD_SEARCH 							0x0063 //指定 Ram Buffer 中的模板与指纹库中指定编号范围内的所有模板之间进行 1:N 比对
#define  CMD_VERIFY 							0x0064 //指定 Ram Buffer 中的指纹模板与指纹库中指定编号的指纹模板之间进行 1:1 比对
//#define  CMD_SET_MODULE_SN 				0x0008 //在设备中设置模块序列号信息（Module SN）
//#define  CMD_GET_MODULE_SN 				0x0009 //获取本设备的模块序列号（ Module SN）
//#define  CMD_FP_CANCEL 						0x0025 //取消指纹采集操作（只适用于带 TimeOut 参数的滑动传感器）
//#define  CMD_GET_ENROLLED_ID_LIST	0x0049 //获取已注册 ID 列表
//#define  CMD_ENTER_STANDY_STATE		0x000C //使模块进入休眠状态。

typedef enum {
	QS808_STATE_IDLE    = 0,
	QS808_STATE_BUSY    = 1,
	QS808_STATE_OK      = 2,
	QS808_STATE_READY   = 3,
	QS808_STATE_FAIL    = 4, 
	QS808_STATE_TIMEOUT = 5,
	QS808_STATE_PARAM   = 6,
}QS808_Status;


typedef enum {
	QS808_Id                = 0,
	QS808_Security_Level    = 1,
	QS808_Duplication_Check = 2,
	QS808_Baudrate          = 3,
	QS808_Auto_Learn        = 4, 
	//QS808_Fp_TimeOut        = 5,//滑动指纹传感器模块
}QS808_Parameter;

/*
ID                表示本设备编号。可设置 1 ~ 255 
Security_Level    表示安全等级。可设置值：1~5 。默认为：3
Duplication_Check 指纹重复检查状态开/关。可设置 0 或 1
Baudrate          1:9600bps, 2:19200bps, 3:38400bps, 4:57600bps, 5:115200bps 6:230400bps, 7:460800bps, 8:921600bps 默认5
Auto_Learn        表示指纹模板自学习状态开/关。可设置 0 或 1
Fp TimeOut        表示采集指纹超时时间，可设置值：1 秒至 60 秒
*/



typedef struct{
	uint16_t prefix;
	uint8_t  sid;
	uint8_t  did;
	uint16_t cmd;
	uint16_t len;
	uint8_t  data[16];
	uint16_t cks;
}QS808_CMD_Type;


typedef struct{
	uint16_t prefix;
	uint8_t  sid;
	uint8_t  did;
	uint16_t rcm;
	uint16_t len;
	uint16_t ret;
	uint8_t  data[14];
	uint16_t cks;
}QS808_CMD_Reply_Type;


typedef struct{
	uint16_t prefix;
	uint8_t  sid;
	uint8_t  did;
	uint16_t cmd;
	uint16_t len;
	uint8_t  data[500];
	uint16_t cks;
}QS808_DATA_Type;

typedef struct{
	uint16_t prefix;
	uint8_t  sid;
	uint8_t  did;
	uint16_t cmd;
	uint16_t len;
	uint16_t ret;
	uint8_t  data[498];
	uint16_t cks;
}QS808_DATA_Reply_Type;

#define QS808_MAX_BUF_SIZE  (sizeof(QS808_CMD_Type)+5)

class FingerPrint : public safety {
	
public:
	virtual void init(void);
	virtual void reset(void){} // 该函数为空
	virtual int8_t add(uint16_t &id, const uint16_t timeout);
	virtual int8_t remove(const uint16_t id_start, const uint16_t id_end);
	virtual int8_t verify(uint16_t &id, const uint16_t timeout);
	virtual int8_t poll(uint16_t &id);
	virtual int8_t read(uint16_t &id){return 0;}

	void QS808_Init(void);
	void Test(void);
private:
	int8_t Fingerprint_Entry(uint8_t (*menutask)(uint8_t state,uint8_t change),uint16_t *etmp_num);
	int8_t Delete_Fingerprint(uint8_t (*menutask)(void),uint16_t tmp_start,uint16_t tmp_end);
	int8_t Verify_Fingerprint(uint8_t (*menutask)(void),uint16_t *tmp_num);
private:
	QS808_Status QS808_Test_Connection(void);
	QS808_Status QS808_Set_Parameter(QS808_Parameter type,uint8_t val);
	QS808_Status QS808_Get_Parameter(QS808_Parameter type);
	//QS808_Status QS808_Get_Device_Info(void);
	QS808_Status QS808_Get_Image(void);
	QS808_Status QS808_Finger_Detect(void);
	QS808_Status QS808_Store_Char(uint16_t tmp_num,uint16_t ram_num);
	QS808_Status QS808_Del_Char(uint16_t tmp_start,uint16_t tmp_end);
	QS808_Status QS808_Get_Empty_Id(uint16_t tmp_start,uint16_t tmp_end);
	QS808_Status QS808_Get_Status(uint16_t tmp_num);//返回OK标示已经注册，FAIL没有注册
	QS808_Status QS808_Get_Broken_Id(uint16_t tmp_start,uint16_t tmp_end);
	QS808_Status QS808_Get_Enroll_Count(uint16_t tmp_start,uint16_t tmp_end);
	QS808_Status QS808_Generate(uint16_t ram_num);
	QS808_Status QS808_Merge(uint16_t ram_num,uint8_t count);
	QS808_Status QS808_Match(uint16_t ram_num1,uint16_t ram_num2);
	QS808_Status QS808_Search(uint16_t ram_num,uint16_t tmp_start,uint16_t tmp_end);
	QS808_Status QS808_Verify(uint16_t tmp,uint16_t ram_num);
	QS808_Status QS808_Cmd_Response(uint8_t *parameter);
	QS808_Status QS808_Load_Char(uint16_t tmp_num,uint16_t ram_num);
};

extern uint8_t menutask(uint8_t state,uint8_t change);

#endif
