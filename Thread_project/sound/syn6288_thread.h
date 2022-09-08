#ifndef    _SYN6288_THREAD_H
#define    _SYN6288_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <math.h>

#include "../main/main_a.h"

#define	UART_DEV	"/dev/ttyS4"
#define	BUSY_DEV	"/sys/class/gpio/gpio125" //忙碌检测引脚，低电平表示语音模块空闲，高电平表示语音模块忙碌
#define DATA_SIZE	14


#define SYN_USART_BAUD          9600
#define FRAME_HEAD		0xFD	//帧头
#define SPEECH_SYSTHESIS_CMD    0x01 //语音合成播放命令字
#define GB2132_WITHOUT_BGM      0x00  //命令参数->不加背景音乐，文本GB2312编码格式
#define GBK_WITHOUT_BGM         0x01  //命令参数->不加背景音乐，文本GBK编码格式
#define Unicode_WITHOUT_BGM     0x03  //命令参数->不加背景音乐，文本Unicode编码格式

void* syn6288(void* thread_arg);

#endif