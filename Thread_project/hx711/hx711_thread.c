#include "hx711_thread.h"

int fd, retvalue;
int weight_maopi = 0;
int weight_shiwu = 0;
double weight_double = 0;


static pthread_once_t once = PTHREAD_ONCE_INIT;

static void hx711_init(void) 
{
	char *filename;
	filename = "/dev/hx711";
	//m=atof(argv[2]);

	/* 打开hx711驱动 */
	fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("file open failed!\r\n");
		exit(-1);
	}
}

static void func_once(void) 
{
 	pthread_once(&once, hx711_init);//执行一次性初始化函数
 	printf("函数open:/dev/hx711 执行一次完毕.\n");
}



void* hx711(void* thread_arg) 
{
	struct thread_data *my_data =  (struct thread_data *) thread_arg;
	int i = 0;

	func_once();

	weight_maopi = 1;
	read(fd, &weight_maopi, sizeof(int));//读取毛皮
	printf("初始化重量数据为： = %d\r\n",weight_maopi);

	while(1)
	{
		usleep(100000);//延时100ms

		my_data->last_weight = weight_double;

		read(fd, &weight_shiwu, sizeof(int));//读取净重
		weight_double = weight_shiwu-weight_maopi;//获取净重量，并记录
		weight_double = weight_double/100;
		weight_double = (unsigned int)((float)weight_double/2.25 + 0.05);//每一个传感器需要矫正4.30这个除数。当发现测试出来的重量偏大时，增加该数值。如果测试出来的重量偏小时，减小改数值.该数值一般在4.0-5.0之间。因传感器线性斜率不同而定。			

		if(my_data->last_weight == weight_double)
		{	
			i++;
			if(i == 15)
			{
				pthread_mutex_lock(&m_mutex);
				my_data->weight = weight_double;
				pthread_mutex_unlock(&m_mutex);
				//pthread_cond_signal(&cond);//向条件变量发送信号
				printf("weight = %.4fg\r\n",weight_double);
				i = 0;
			}
		}	

	}

	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file close failed!\r\n");
		return (void*)-1;
	}
	return NULL;
}
