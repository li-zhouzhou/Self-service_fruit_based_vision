#include "syn6288_thread.h"

char Data[5] = {FRAME_HEAD, SPEECH_SYSTHESIS_CMD, GB2132_WITHOUT_BGM, GBK_WITHOUT_BGM,Unicode_WITHOUT_BGM};
int busy_fd = -1;
int uart_fd = -1;
int SynBusy_Sta = 1;//忙碌检测

//字符编码转换
int Utf8ToUnicode(char* pInput, char* pOutput)
{
    int outputSize = 0; //记录转换后的Unicode字符串的字节数
    while (*pInput)  
    {  
        if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）  
        {  
            *pOutput = *pInput;  
            pOutput++;  
            *pOutput = 0; //小端法表示，在高地址填补0  
        }  
        else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符  
        {  
            char high = *pInput;  
            pInput++;  
            char low = *pInput;  
            if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示  
            {  
                return -1; //如果不是则报错  
            }  
    
            *pOutput = (high << 6) + (low & 0x3F);  
            pOutput++;  
            *pOutput = (high >> 2) & 0x07;  
        }  
        else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符  
        {  
            char high = *pInput;  
            pInput++;  
            char middle = *pInput;  
            pInput++;  
            char low = *pInput;  
            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))  
            {  
                return -1;  
            }  
            *pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位  
            pOutput++;  
            *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位  
        }  
        else //对于其他字节数的UTF8字符不进行处理  
        {  
            return -1;  
        }  
        pInput ++;//处理下一个utf8字符  
        pOutput ++;  
        outputSize += 2;  
    }  
    //unicode字符串后面，有两个\0  
    *pOutput = 0;  
    pOutput++;  
    *pOutput = 0;  
    return outputSize; 
} 

//************************************************************************************88

//语音合成
//芯片支持以下命令帧格式:“帧头FD+数据区长度+数据区”格式。（最大206个字节）
//上位机发送给SYN6288芯片的所有命令和数据都需要用"帧"的方式进行封装后传输
void SYN6288SpeechSynthesis(char *str)
{
	char *p = NULL;
        p = str;
	int len = 0, i;
	int ret;
	char len_data1;
	char len_data2;
	char checksum;//异或校验

  /* 存放转换后的字符串 */
    int n;
    char outbuf[1024] = {};
    size_t outlen = 1024;

    n = Utf8ToUnicode(str,outbuf);
    //printf ("n: %d\n", n);
    for(i=0;i<n;i=i+2)
    {
        char m;
        m=outbuf[i];
        outbuf[i]=outbuf[i+1];
        outbuf[i+1]=m;  
    }

    len = n + 3;//计算数据区长度
    //printf("数据区长度: %d\n", len);
    
    //等待CHIP空闲，低电平表示空闲
    // while(SynBusy_Sta);
    usleep(2000);/*测试得出这里要加至少2ms的延时，不然如果连续发送语音会发生某些语音丢失的情况，
                  只延时1ms也会丢失,直接加载后面的8ms上也会发生丢失的情况*/

	write(uart_fd, &Data[0], 1);//发送帧头
	len_data1 = len >> 8;
	len_data2 = len;
	write(uart_fd, &len_data1, 1);//发送数据区长度高八位
	write(uart_fd, &len_data2, 1);	
		
	checksum = FRAME_HEAD ^ (len >> 8) ^ (len);//校验和
    
	write(uart_fd, &Data[1], 1);//发送语音合成播放命令字
	write(uart_fd, &Data[4], 1);//发送命令参数->不加背景音乐，文本GBK编码格式
	checksum = checksum ^ SPEECH_SYSTHESIS_CMD ^ Data[4];
	//printf("checksum_value1: %x\n", checksum);
	
	//发送文本内容
	for(i = 0; i < n; i++)
	{
		write(uart_fd, &outbuf[i], 1);
		//printf ("print outbuf: %x\n", outbuf[i]);
		checksum ^= outbuf[i];
	}

	//printf("checksum_value2: %x\n", checksum);	
	write(uart_fd, &checksum, 1);//发送异或检验
	sleep(1);//数据手册要求帧与帧之间的发送间隔必须超过8ms
}


int uart_open(const char *dev_name)
{
	int ret = 0;
	struct termios uart_opt = { 0 };
	int fd = open(dev_name, O_RDWR | O_NOCTTY);
	if(fd < 0){
		perror("Open");
		return -1;
	}
	
	uart_opt.c_cflag |= CLOCAL | CREAD;
	
	uart_opt.c_cflag &= ~CSIZE;
	uart_opt.c_cflag |= CS8;

	uart_opt.c_cflag &=~PARENB;

	uart_opt.c_cflag &=~CSTOPB;

	uart_opt.c_cflag &=~CRTSCTS;
	uart_opt.c_cc[VTIME] = 6;
	uart_opt.c_cc[VMIN] = 0;
	
	cfsetispeed(&uart_opt, B9600);
	cfsetospeed(&uart_opt, B9600);
	ret = tcsetattr(fd, TCSANOW, &uart_opt);
	if(ret < 0)
	{
		return -1;
	}
	return fd;
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=

static char gpio_path[100];

static int gpio_config(const char *attr, const char *val)
{
    char file_path[100];
    int len;
    int fd;

    sprintf(file_path, "%s/%s", BUSY_DEV, attr);
    if (0 > (fd = open(file_path, O_WRONLY))) {
        perror("open error");
        return fd;
    }

    len = strlen(val);
    if (len != write(fd, val, len)) {
        perror("write error");
        close(fd);
        return -1;
        close(fd);
        return -1;
    }

    close(fd);  //关闭文件
    return 0;
}

void busy_gpio_init()
{
    char file_path[100];
    char val;
    int fd;

    /* 判断指定编号的GPIO是否导出 
    if (access(BUSY_DEV, F_OK)) {//如果目录不存在 则需要导出

        int len;

        if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) {
            perror("open error");
            exit(-1);
        }

        len = strlen("125");
        if (len != write(fd, "125", len)) {//导出gpio
            perror("write error");
            close(fd);
            exit(-1);
        }
        close(fd);  //关闭文件
    }

    /* 配置为输入模式 
    if (gpio_config("direction", "in"))
        exit(-1);

    /* 极性设置 
    if (gpio_config("active_low", "0"))
        exit(-1);

    /* 配置为非中断方式 
    if (gpio_config("edge", "none"))
        exit(-1);

    /* 读取GPIO电平状态 
    sprintf(file_path, "%s/%s", BUSY_DEV, "value");

    if (0 > (busy_fd = open(file_path, O_RDONLY))) {
        perror("open error");
        exit(-1);
    }

    if (0 > read(busy_fd, &val, 1)) {
        perror("read error");
        close(fd);
        exit(-1);
    }

    printf("value: %c\n", val);

}
********************************************************/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void* syn6288(void* thread_arg)
{
    struct thread_data *my_data =  (struct thread_data *) thread_arg;

    char str[100] = {0}; 

	uart_fd = uart_open(UART_DEV);
	if ( uart_fd < 0 )
	{
	    printf("uart_fd  open fail!!!");
	    return (void*)1;
	}

    char last_syn62888_type[10];
    int last_syn6288_weight;

    strcpy(last_syn62888_type, my_data->type);
    last_syn6288_weight = my_data->weight;
    SYN6288SpeechSynthesis("中国研究生电子设计大赛");

	while (1)
	{ 
        if(strcmp(last_syn62888_type,my_data->type) || abs(last_syn6288_weight - my_data->weight) > 10)
        {
            if(my_data->weight < 10)
            {
                SYN6288SpeechSynthesis("当前无任何水果");   
            }
            else
            {
                memset(str, 0, 100);
                if(strcmp(my_data->type,"bananan") == 0)
                {
                    sprintf(str, "香蕉的重量是%d克\n",my_data->weight);
                }
                else if(strcmp(my_data->type,"mango")== 0)
                {
                    sprintf(str, "芒果的重量是%d克\n",my_data->weight);
                }
                else if(strcmp(my_data->type,"pear")== 0)
                {
                    sprintf(str, "梨的重量是%d克\n",my_data->weight);
                }
                else if(strcmp(my_data->type,"nectarinee")== 0)
                {
                    sprintf(str, "油桃的重量是%d克\n",my_data->weight);
                }
                else if(strcmp(my_data->type,"apple")== 0)
                {
                    sprintf(str, "苹果的重量是%d克\n",my_data->weight);
                }
                //SYN6288SpeechSynthesis("中国研究生电子设计大赛");
                printf("my_data->type: %s", my_data->type);
                SYN6288SpeechSynthesis(str);
            }

        }

        strcpy(last_syn62888_type, my_data->type);
        last_syn6288_weight = my_data->weight;
	    sleep(4);
	}

	close(uart_fd);
	close(busy_fd);

	return NULL;
}





