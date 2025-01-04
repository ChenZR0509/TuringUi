/**
  *@ FileName: SSD1306.c
  *@ Author: CzrTuringB
  *@ Brief: IIC接口的SSD1306源文件
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
/* Includes ------------------------------------------------------------------*/
#include "Ssd1306.h"
#include "DataStructure/Queue.h"
#include "BspState.h"
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
#define	SSD1306I2cHandle hi2c1			//IIC接口
#define SSD1306Address 0x78				//从机地址
#define cmdBufferSize  30				//命令流数组元素个数
//-variable
static uint8_t cmdBuffer[cmdBufferSize]; //创建命令数组，用于存储SSD1306的命令流
//-struct
typedef struct
{
	SsdPublicConfig publicConfig;       			//公有部分
	Queue cmdQueue;									//定义命令队列
	uint8_t markBlock[16];							//缓冲块更新标记数组,一个字节有八位，每位1表示块更新，0表示块没更新
	uint32_t lastCrc[8][16];        				//记录每个块的CRC校验码
	SemaphoreHandle_t dmaIdle;						//DMA空闲判断
}SsdConfigStruct;
static SsdConfigStruct sUiDevice;						//静态结构体变量
SsdPublicConfig* uiDevice = &sUiDevice.publicConfig;	//结构体共有部分指针变量
//-enum
typedef enum
{
	Data = 0x40,
	Command = 0x00,
}CommunicationMode;
typedef enum
{
	DisplayOn = 0xAF,
	DisplayOff = 0xAE,
}DisplayState;
typedef enum
{
	HorizontalMode = 0,	//水平寻址
	VerticalMode = 1,	//垂直寻址
	PageMode = 2,		//页寻址
}AddressMode;
typedef enum
{
	MapOrder = 0xA0,	//顺序映射
	MapReverse = 0xA1,	//逆序映射
}ColumnMap;
typedef enum
{
	ScanOrder = 0xC0,	//顺序扫描
	ScanReverse = 0xC8,	//逆序扫描
}ComScan;
typedef enum
{
	SnLR = 0x02,	//COM与引脚顺序映射
	SLR	 = 0x22,	//COM与引脚顺序映射，芯片左右两边映射颠倒
	AnLR = 0x12,	//COM与引脚自选映射
	ALR	 = 0x32,	//COM与引脚自选映射，芯片左右两边映射颠倒
}ComConfig;
/* Functions ------------------------------------------------------------------*/
void SSDSetPageAddress(uint8_t pageStart,uint8_t pageEnd);
void SSDSetColumnAddress(uint8_t columnStart,uint8_t columnEnd);
/* Functions ------------------------------------------------------------------*/
/**
  *@ FunctionName: void SSDWriteCmdByte(uint8_t cmd)
  *@ Author: CzrTuringB
  *@ Brief: 向SSD1306芯片中写入一个命令
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、阻塞式传输
  *@ 	2、尽量不要使用这个API
  */
void SSDWriteCmdByte(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&SSD1306I2cHandle, SSD1306Address, Command, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
}
/**
  *@ FunctionName: void SSDWriteDataByte(uint8_t data)
  *@ Author: CzrTuringB
  *@ Brief: 向SSD1306芯片中写入一个字节数据
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、阻塞式传输
  *@ 	2、尽量不要使用这个API
  */
void SSDWriteDataByte(uint8_t data)
{
	HAL_I2C_Mem_Write(&SSD1306I2cHandle, SSD1306Address, Data, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}
/**
  *@ FunctionName: void SSDWriteCmds(void)
  *@ Author: CzrTuringB
  *@ Brief: 使用DMA技术发送多个命令字节
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDWriteCmds(void)
{
	HAL_I2C_Mem_Write_DMA(&SSD1306I2cHandle, SSD1306Address, Command, I2C_MEMADD_SIZE_8BIT, cmdBuffer, sUiDevice.cmdQueue.count);
	xSemaphoreTake(sUiDevice.dmaIdle,portMAX_DELAY);
	sUiDevice.cmdQueue.head = 0;
	sUiDevice.cmdQueue.tail = 0;
	sUiDevice.cmdQueue.count = 0;
}
/**
  *@ FunctionName: void SSDBlockDetect(void)
  *@ Author: CzrTuringB
  *@ Brief: 区域块数据变更检测
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
void SSDBlockDetect(void)
{
    for (uint8_t row = 0; row < 8; row++)
    {
        uint16_t rowMark = 0; // 用16位变量表示一行的更新标记

        for (uint8_t col = 0; col < 16; col++)
        {
            uint8_t* pBlockData = &uiDevice->disBuffer[row][col * 8]; // 获取块数据指针

            // 使用HAL_CRC_Calculate计算CRC
            uint32_t currentCrc = HAL_CRC_Calculate(&hcrc, (uint32_t *)pBlockData, 2); // 计算CRC
            // 判断当前CRC是否和上次的CRC相同，如果不同则标记该块为已更新
            if (currentCrc != sUiDevice.lastCrc[row][col])
            {
            	sUiDevice.lastCrc[row][col] = currentCrc; // 更新CRC记录
                rowMark |= (1 << col); // 在对应位置标记更新
            }
        }

        // 将16位行标记写入对应的两个 `markBlock` 元素
        sUiDevice.markBlock[row * 2] = (uint8_t)(rowMark & 0xFF);         // 低8位
        sUiDevice.markBlock[row * 2 + 1] = (uint8_t)((rowMark >> 8) & 0xFF); // 高8位
    }
}
/**
  *@ FunctionName: void SSDWriteDataAll()
  *@ Author: CzrTuringB
  *@ Brief: 使用DMA技术发送整个数据缓冲区
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDWriteDataAll(void)
{
	SSDSetPageAddress(0, 7);
	SSDSetColumnAddress(0, 127);
	SSDWriteCmds();
	HAL_I2C_Mem_Write_DMA(&SSD1306I2cHandle, SSD1306Address, Data, I2C_MEMADD_SIZE_8BIT, (uint8_t*)(uiDevice->disBuffer), 1024);
	xSemaphoreTake(sUiDevice.dmaIdle,portMAX_DELAY);
	SSDBlockDetect();								//更新CRC
    for (uint8_t i = 0; i < 16; i++)				//去除标记
    {
    	sUiDevice.markBlock[i] = 0;
    }
}
/**
  *@ FunctionName: void SSDUpdateScreen(void)
  *@ Author: CzrTuringB
  *@ Brief: 屏幕刷新函数
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
void SSDUpdateScreen(void)
{
    SSDBlockDetect(); // 检测哪些块需要更新
    for (uint8_t page = 0; page < 8; page++) // 遍历每个页
    {
        uint16_t pageMark = ((uint16_t)sUiDevice.markBlock[page * 2 + 1] << 8) | sUiDevice.markBlock[page * 2]; // 当前页的更新标记
        while (pageMark) // 只处理有标记的部分
        {
            // 找到最低位的1（起始列块）
            uint8_t startCol = __builtin_ctz(pageMark); // 获取最低位1的索引

            // 找到连续的1块
            uint16_t mask = (1 << startCol);
            uint8_t endCol = startCol; // 默认结束列为起始列

            // 扩展连续的1块
            while (pageMark & mask)
            {
                mask <<= 1; // 扩展范围

                // 如果 mask 超过有效位数（16位），则退出
                if (mask == 0)
                {
                    break;  // 防止超出有效位范围
                }
            }

            // 如果 mask 变为 0，表示已经处理到最后一列，endCol应该是15
            if (mask == 0)
            {
                endCol = 15; // 确保最后列的 endCol 为 15
            } else
            {
                endCol = __builtin_ctz(mask) - 1; // 结束列块
            }
            // 计算起始和结束地址
            uint8_t startColAddr = startCol * 8;
            uint8_t endColAddr = endCol * 8 + 7;

            // 设置页和列范围
            SSDSetPageAddress(page, page); // 设置页地址
            SSDSetColumnAddress(startColAddr, endColAddr); // 设置列地址范围
            SSDWriteCmds();

            // 发送连续块列的数据
            uint8_t* pBlockData = &uiDevice->disBuffer[page][startColAddr];
            uint16_t dataLength = (endCol - startCol + 1) * 8;
            HAL_I2C_Mem_Write_DMA(&SSD1306I2cHandle, SSD1306Address, Data, I2C_MEMADD_SIZE_8BIT, pBlockData, dataLength);
            xSemaphoreTake(sUiDevice.dmaIdle, portMAX_DELAY); // 等待DMA完成

            // 清除已处理的块标记
            uint16_t clearMask = ((1 << (endCol - startCol + 1)) - 1) << startCol; // 清除连续标记位
            pageMark &= ~clearMask; // 更新 pageMark，清除已处理标记
        }

        // 清除 `markBlock` 的对应标记位
        sUiDevice.markBlock[page * 2] = 0;     // 清零低8位
        sUiDevice.markBlock[page * 2 + 1] = 0; // 清零高8位
    }
}
/**
  *@ FunctionName: void SSDPower(DisplayState cmd)
  *@ Author: CzrTuringB
  *@ Brief: 打开\关闭Oled的显示
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDPower(DisplayState cmd)
{
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
}
/**
  *@ FunctionName: void SSDClean(void)
  *@ Author: CzrTuringB
  *@ Brief: 清空OLED的显示
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDClean(void)
{
	uint8_t temp;
	if (uiDevice->dataValid == False)
	{
		temp = 0x00;
	}
	else
	{
		temp = 0xFF;
	}
	for(uint8_t x = 0; x<128 ; x++)
	{
		for(uint8_t y = 0; y<8 ; y++)
		{
			uiDevice->disBuffer[y][x] = temp;
		}
	}
}
/**
  *@ FunctionName: void SSDClkConfig(uint8_t divide,uint8_t fosc)
  *@ Author: CzrTuringB
  *@ Brief:	配置SSD1306芯片的时钟
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、设置时钟分频系数Divide，其范围是：0xX0~0xXF
  *@ 	2、设置时钟频率Fosc，其范围是：0x0X~0xFX
  *@ 	3、一般只有在初始化的时候调用这个函数，并且其配置为：Divide=0x00,Fosc=0x80
  */
void SSDClkConfig(uint8_t divide,uint8_t fosc)
{
	uint8_t result = (0x0F&divide)|(0xF0&fosc);
	uint8_t cmd = 0xD5;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &result);
}
/**
  *@ FunctionName: void SSDSetMux(uint8_t mux)
  *@ Author: CzrTuringB
  *@ Brief:	设置显示映射到显存的范围,即有显存中有多少行数据映射到显示器上
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、设置显示范围Mux，其范围是：0x00~0x3F
  *@ 	2、显示范围对应的是RAM行，设置后RAM行范围：0~Mux
  */
void SSDSetMux(uint8_t mux)
{
	uint8_t cmd = 0xA8;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &mux);
}
/**
  *@ FunctionName: SSDSetOffset(uint8_t offset)
  *@ Author: CzrTuringB
  *@ Brief:	设置显示的哪个行是起始行
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、偏移量Offset，其范围是：0x00~0x3F
  *@ 	2、偏移量指的是COM0对应的那个Offset显示行
  */
void SSDSetOffset(uint8_t offset)
{
	uint8_t cmd = 0xD3;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &offset);
}
/**
  *@ FunctionName: void SSDSetStartline(uint8_t startline)
  *@ Author: CzrTuringB
  *@ Brief: 设置显示的起始行对应显存中的第几行
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、起始行Startline，其范围是0x40~0x7F
  *@ 	2、显示起始线指的是Row0对应的那个RamRow
  */
void SSDSetStartline(uint8_t startline)
{
	QueueInElement(&sUiDevice.cmdQueue, &startline);
}
/**
  *@ FunctionName: void SSDColConfig(ColumnMap mode)
  *@ Author: CzrTuringB
  *@ Brief:	设置列Cloumn与段Segment的映射模式
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、设置显示列与显存列的对应情况
  *@ 	2、顺序：显示列0对应显存列0
  *@ 	3、逆序：显示列0对应显存列127
  */
void SSDColConfig(ColumnMap mode)
{
	QueueInElement(&sUiDevice.cmdQueue, &mode);
}
/**
  *@ FunctionName: void SSDComScan(ComScan mode)
  *@ Author: CzrTuringB
  *@ Brief: 设置COM的扫描方式
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDComScan(ComScan mode)
{
	QueueInElement(&sUiDevice.cmdQueue, &mode);
}
/**
  *@ FunctionName: void SSDComConfig(ComConfig mode)
  *@ Author: CzrTuringB
  *@ Brief: SSD的COM与引脚的映射配置
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDComConfig(ComConfig mode)
{
	uint8_t cmd = 0xDA;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &mode);
}
/**
  *@ FunctionName: void SSDSetContrast(uint8_t contrast)
  *@ Author: CzrTuringB
  *@ Brief:	设置显示对比度
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、设置对比度contrast，其范围是：0x00~0xff
  */
void SSDSetContrast(uint8_t contrast)
{
	uint8_t cmd = 0x81;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &contrast);
}
/**
  *@ FunctionName: void SSDPreChargeConfig(uint8_t phase1,uint8_t phase2)
  *@ Author: CzrTuringB
  *@ Brief:	配置SSD提前充电周期
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、Phase1：表示阶段1的充电周期，其范围是0xX0~0xXF
  *@ 	2、Phase2：表示阶段2的充电周期，其范围是0x0X~0XFX
  */
void SSDPreChargeConfig(uint8_t phase1,uint8_t phase2)
{
	uint8_t result = (0x0F&phase1)|(0xF0&phase2);
	uint8_t cmd = 0xD9;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &result);
}
/**
  *@ FunctionName: void SSDComhConfig(uint8_t level)
  *@ Author: CzrTuringB
  *@ Brief: 配置Comh引脚取消选择电压等级
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、等级：0x00 0x20 0x30
  */
void SSDComhConfig(uint8_t level)
{
	uint8_t cmd = 0xDB;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &level);
}
/**
  *@ FunctionName: void SSDSetHold(EnDis holdOn)
  *@ Author: CzrTuringB
  *@ Brief: 设置屏幕显示是否保持不变
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、输入：Enale or Disable
  */
void SSDSetHold(EnDis holdOn)
{
	uint8_t cmd;
	if(holdOn == Enable)
	{
		cmd = 0xA5;
		//保持屏幕内容不变，显示输出不跟随显存
		QueueInElement(&sUiDevice.cmdQueue, &cmd);
	}
	else
	{
		cmd = 0xA4;
		//显示输出跟随显存
		QueueInElement(&sUiDevice.cmdQueue, &cmd);
	}
}
/**
  *@ FunctionName: void SSDSetReverse(EnDis reverse)
  *@ Author: CzrTuringB
  *@ Brief: 设置屏幕翻转显示
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、输入：Enale or Disable
  */
void SSDSetReverse(EnDis reverse)
{
	if(reverse == Enable)
	{
		//0 表示显示	1表示不显示
		uint8_t cmd = 0xA7;
		uiDevice->dataValid = True;
		QueueInElement(&sUiDevice.cmdQueue, &cmd);
	}
	else
	{
		//1 表示显示	0表示不显示
		uint8_t cmd = 0xA6;
		uiDevice->dataValid = False;
		QueueInElement(&sUiDevice.cmdQueue, &cmd);
	}
}
/**
  *@ FunctionName: void SSDPreChargePump(void)
  *@ Author: CzrTuringB
  *@ Brief:	使能充电调节
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void SSDChargePump(void)
{
	uint8_t cmd = 0x8D;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	cmd = 0x14;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
}
/**
  *@ FunctionName: void SSDAddressConfig(AddressMode mode)
  *@ Author: CzrTuringB
  *@ Brief: 配置寻址模式
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、除非特别设置，默认都为水平寻址模式
  */
void SSDAddressConfig(AddressMode mode)
{
	uint8_t cmd = 0x20;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	QueueInElement(&sUiDevice.cmdQueue, &mode);
}
/**
  *@ FunctionName: void SSDSetSEColumnAddress(uint8_t columnStart,uint8_t columnEnd)
  *@ Author: CzrTuringB
  *@ Brief:	设置起始列地址和终止列地址
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、数据范围：0~127
  *@ 	2、适用于非页寻址模式
  */
void SSDSetColumnAddress(uint8_t columnStart,uint8_t columnEnd)
{
	uint8_t cmd = 0x21;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	columnStart = 0x7F & columnStart;
	QueueInElement(&sUiDevice.cmdQueue, &columnStart);
	columnEnd = 0x7F & columnEnd;
	QueueInElement(&sUiDevice.cmdQueue, &columnEnd);
}
/**
  *@ FunctionName: void SSDSetSEPageAddress(uint8_t PageStart,uint8_t PageEnd)
  *@ Author: CzrTuringB
  *@ Brief:	设置起始页地址和终止页地址
  *@ Time: Sep 20, 2024
  *@ Requirement：
  *@ 	1、数据范围：0~7
  *@ 	2、适用于非页寻址模式
  */
void SSDSetPageAddress(uint8_t pageStart,uint8_t pageEnd)
{
	uint8_t cmd = 0x22;
	QueueInElement(&sUiDevice.cmdQueue, &cmd);
	pageStart = 0x07 & pageStart;
	QueueInElement(&sUiDevice.cmdQueue, &pageStart);
	pageEnd = 0x07 & pageEnd;
	QueueInElement(&sUiDevice.cmdQueue, &pageEnd);
}
/**
  *@ FunctionName: void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
  *@ Author: CzrTuringB
  *@ Brief: IIC+DMA发送完成后产生的回调函数
  *@ Time: Sep 20, 2024
  *@ Requirement：
  */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	//当DMA数据发送完后，发送DMA空闲信号表明DMA处于空闲状态
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(sUiDevice.dmaIdle,&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/**
  *@ FunctionName: void SSDUpdateScreen(void)
  *@ Author: CzrTuringB
  *@ Brief: 屏幕刷新函数
  *@ Time: Dec 23, 2024
  *@ Requirement：
  */
void SSDInit(void)
{
	//创建DMA空闲信号量
	sUiDevice.dmaIdle = xSemaphoreCreateBinary();
	//初始化命令缓冲区
	QueueInit(&sUiDevice.cmdQueue, sizeof(uint8_t), cmdBufferSize, cmdBuffer);
	SSDPower(DisplayOff);
	SSDClkConfig(0x00, 0x70);
	SSDSetMux(0x3F);
	SSDSetOffset(0x00);
	SSDSetStartline(0x40);
	SSDColConfig(MapReverse);
	SSDComScan(ScanOrder);
	SSDComConfig(AnLR);
	SSDSetContrast(0xCF);
	SSDPreChargeConfig(0x01, 0xE0);
	SSDComhConfig(0x30);
	SSDSetHold(Disable);
	SSDSetReverse(Disable);
	SSDAddressConfig(HorizontalMode);
	SSDWriteCmds();
	SSDClean();
	SSDWriteDataAll();
	SSDChargePump();
	SSDPower(DisplayOn);
	SSDWriteCmds();
}
