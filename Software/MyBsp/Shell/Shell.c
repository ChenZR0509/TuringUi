/**
  *@ FileName：Shell.c
  *@ Author：CzrTuringB
  *@ Brief：
  *@ Time：Jan 13, 2025
  *@ Requirement：
  *@ 	1、在FreeRTOS.h中配置configUSE_QUEUE_SETS为1
  *@ Notes：
  */
/* Includes" "------------------------------------------------------------------*/
#include "Shell.h"
#include "Uart/ShellUart.h"
/* Includes< >------------------------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
/* Data(作用域为当前C文件)-----------------------------------------------------*/
//-define
#define MessageQueueSize 	10						//消息队列大小
#define InputQueueSize		80						//接收消息队列大小
//-enum
//-struct
//-variable
static QueueSetHandle_t messageQueueSet = NULL;		//消息集合
static SemaphoreHandle_t messageMutex = NULL;		//消息锁
static QueueHandle_t messageQueue = NULL;			//消息队列
pShellMessage pcMessage = NULL;						//全局指针变量
xTaskHandle ShellTaskHandle = NULL;					//Shell任务句柄
/* Functions Declare------------------------------------------------------------------*/
//-重定向printf
#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *stream)
#endif
PUTCHAR_PROTOTYPE
{
    if (pcMessage != NULL && pcMessage->index < MaxMessageSize - 1)
    {
        // 写入字符到目标结构体
    	pcMessage->messageBuffer[pcMessage->index++] = (char)ch;
    }
    return ch;
}
/* Functions Define------------------------------------------------------------------*/
/**
  *@ FunctionName: pShellMessage MessageCreate(uint8_t reveiver)
  *@ Author: CzrTuringB
  *@ Brief: 动态创建一个消息结构体变量
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
pShellMessage MessageCreate(uint8_t receiver)
{
    //1、动态创建消息结构体变量
    pShellMessage newMessage = (pShellMessage)pvPortMalloc(sizeof(ShellMessage));
    if (newMessage == NULL)	return NULL;
    //2、初始化结构体字段
    newMessage->receiver = receiver;
    newMessage->index = 0;
    memset(newMessage->messageBuffer, 0, MaxMessageSize);
    //3、返回结构体指针
    return newMessage;
}
/**
  *@ FunctionName: BspState MessageDestory(pShellMessage message)
  *@ Author: CzrTuringB
  *@ Brief: 消息对象的删除
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState MessageDestory(pShellMessage message)
{
    //1、检查形参有效性
    if (message == NULL) return BspError;
    //2、释放动态分配的内存
    vPortFree(message);
    message = NULL;
    return BspOk;
}
/**
  *@ FunctionName: BspState MessageReset(pShellMessage message, uint8_t receiver)
  *@ Author: CzrTuringB
  *@ Brief: 消息对象的重置
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState MessageReset(pShellMessage message, uint8_t receiver)
{
    //1、检查形参有效性
    if (message == NULL) return BspError;
    message->index = 0;
    message->receiver = receiver;
    //2、返回
    return BspOk;
}
/**
  *@ FunctionName: BspState ShellInit(void)
  *@ Author: CzrTuringB
  *@ Brief:  初始化Shell
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ShellInit(void)
{
	//1、创建消息队列
	messageQueue = xQueueCreate(MessageQueueSize,sizeof(ShellMessage));
	if(messageQueue == NULL) return BspError;
	//2、创建消息锁
	messageMutex = xSemaphoreCreateMutex();
	if (messageMutex == NULL) return BspError;
	//3、创建UartShell任务(具有接收功能)
	UartInputQueue = xQueueCreate(InputQueueSize,sizeof(uint8_t));
	if (UartInputQueue == NULL) return BspError;
	xTaskHandle ShellUartTaskHandle;
	xTaskCreate((void*)ShellUartApp, "ShellUart", 64 * 4, NULL, 24, &ShellUartTaskHandle);
	//4、创建队列集合(增加队列需要改长度)
	messageQueueSet = xQueueCreateSet(90);
	if (messageQueueSet == NULL) return BspError;
	xQueueAddToSet(messageQueue, messageQueueSet);
	xQueueAddToSet(UartInputQueue, messageQueueSet);
	return BspOk;
}
/**
  *@ FunctionName: BspState ShellPrintf(uint8_t receiver, const char *format, ...)
  *@ Author: CzrTuringB
  *@ Brief: 打印消息
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ShellPrintf(uint8_t receiver, const char *format, ...)
{
	//1、判断形参有效性
    if (format == NULL) return BspError;
    //2、创建消息对象
    pShellMessage message = MessageCreate(receiver);
    if(!message)	return BspError;
	//2、指定全局指针对象(保护不被其他任务或中断访问)
    if (xSemaphoreTake(messageMutex, portMAX_DELAY) != pdTRUE) return BspError;
	//3、初始化消息
	pcMessage = message;
	//4、调用vprintf解析格式化字符串
    va_list args_list;
    va_start(args_list, format);
    int length = vsnprintf((char *)message->messageBuffer, MaxMessageSize, format, args_list);
    va_end(args_list);
    //5、判断字符格式化是否正确
    if (length < 0 || length >= MaxMessageSize)
    {
        xSemaphoreGive(messageMutex);
        return BspError;
    }
    message->index = (uint8_t)length;
    //6、将消息发送到队列
    BaseType_t result = xQueueSendToBack(messageQueue, message, portMAX_DELAY);
    //7、释放锁
    xSemaphoreGive(messageMutex);
    MessageDestory(message);
    return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: BspState ShellNormalPrintf(pShellMessage message, const char *format, ...)
  *@ Author: CzrTuringB
  *@ Brief: 打印消息
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
BspState ShellNormalPrintf(pShellMessage message, const char *format, ...)
{
	//1、判断形参有效性
    if (message == NULL || format == NULL) return BspError;
	//2、指定全局指针对象(保护不被其他任务或中断访问)
    if (xSemaphoreTake(messageMutex, portMAX_DELAY) != pdTRUE) return BspError;
	//3、初始化消息
	pcMessage = message;
	message->index = 0;
	//4、调用vprintf解析格式化字符串
    va_list args_list;
    va_start(args_list, format);
    int length = vsnprintf((char *)message->messageBuffer, MaxMessageSize, format, args_list);
    va_end(args_list);
    //5、判断字符格式化是否正确
    if (length < 0 || length >= MaxMessageSize)
    {
        xSemaphoreGive(messageMutex);
        return BspError;
    }
    message->index = (uint8_t)length;
    //6、将消息发送到队列
    BaseType_t result = xQueueSendToBack(messageQueue, message, portMAX_DELAY);
    message = NULL;
    //7、释放锁
    xSemaphoreGive(messageMutex);
    return result == pdPASS ? BspOk : BspError;
}
/**
  *@ FunctionName: void ShellOutput(void)
  *@ Author: CzrTuringB
  *@ Brief: Shell输出处理
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  *@ 	1、将打印信息依据接收方发送出去
  */
void ShellOutput(void)
{
	ShellMessage message;
	if(xQueueReceive(messageQueue, &message, portMAX_DELAY) == pdPASS)
	{
		if(message.receiver & 0x01)
		{
			xQueueSendToBack(UartOutputQueue, &message, portMAX_DELAY);
		}
	}
}
/**
  *@ FunctionName: void ShellIntput()
  *@ Author: CzrTuringB
  *@ Brief: Shell输入处理
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:【一个新的项目TuringShell】
  *@ 	1、依据获取到的命令信息执行相应的代码
  *@ 	2、哈系表
  *@ 	3、trie树
  */
void ShellIntput(void)
{
	static ShellMessage message;
	if(xQueueReceive(UartInputQueue, (void*)&message.messageBuffer[message.index], portMAX_DELAY) == pdPASS)
	{
        if (message.messageBuffer[message.index] != '\0')
        {
        	message.index++;
        }
        else
        {
        	ShellPrintf(UartShell, "%s", message.messageBuffer);
        	message.index = 0;
        }
	}
}
/**
  *@ FunctionName: void ShellApp(void)
  *@ Author: CzrTuringB
  *@ Brief: Shell任务函数
  *@ Time: Jan 14, 2025
  *@ Requirement：
  *@ Notes:
  */
void ShellApp(void)
{
	//1、初始化Shell
	ShellInit();
	//2、创建队列集合
	QueueSetMemberHandle_t member;
	while(1)
	{
		//1、从队列集合中接收消息
		member = xQueueSelectFromSet(messageQueueSet, portMAX_DELAY);
		//2、从messageQueue中取出了一个消息，依据消息接收方在不同的外设输出
		if(member == messageQueue) ShellOutput();
		//3、从UartInputQueue取出了一个消息，依据相关命令执行不同的功能
		if(member == UartInputQueue) ShellIntput();
	}
}
