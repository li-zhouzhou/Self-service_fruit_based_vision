#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#define NEWCHRLED_CNT			       1		  	/* 设备号个数 */
#define NEWCHRLED_NAME			"hx711"	/* 名字 */
#define LEDOFF 					0			/* 关灯 */
#define LEDON 					1			/* 开灯 */
 


/* newchrled设备结构体 */
struct newchrled_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;		/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
};

struct newchrled_dev newchrled;	/* led设备 */



struct GPIO
{
    int gpio;
    char name[10];
};

                         //GPIO7_23  //GPIO7_22  
struct GPIO led_info[2]={{215,"SCK"},{214,"SDA"}};

//---------------------------------------------------------------------------------------------------------------
void HX711_SCL1_L(void)
{
    gpio_set_value(led_info[0].gpio, 0);
}

void HX711_SCL1_H(void)
{
    gpio_set_value(led_info[0].gpio, 1);
}

static int HX711_SDA1_IN(void)
{
    int a;
    a = gpio_get_value(led_info[1].gpio);
    return a;
}

unsigned long HX711_Read(void)  //读取711读到的数据
{
    unsigned long val; 
    unsigned char i; 	
    HX711_SCL1_L(); 
    val = 0;
    while(HX711_SDA1_IN());
    udelay(1); 
    for(i=0;i<24;i++){ 
		HX711_SCL1_H(); 
		val=val<<1; 
		udelay(1);
		HX711_SCL1_L(); 
		if(HX711_SDA1_IN()) 
		    val++; 
		udelay(1);
    } 
    HX711_SCL1_H(); 
    val = val^0x800000; 
    udelay(1);
    HX711_SCL1_L(); 
    udelay(1);
	
    return val; 	
}


//-----------------------------------------------------------------------------------------------------

static int hx711_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t hx711_write(struct file *file, const char __user *buffer, size_t count, loff_t *offset)
{
	return 0;
}


static ssize_t hx711_read(struct file *file, char __user *buffer, size_t count, loff_t *offset)
{
	ssize_t ret = 0;
	int adc_data;
	printk("hx711_read\n");
	//return heartbeat_read(file,buffer,count,offset);
	adc_data = HX711_Read();
	ret =  copy_to_user(buffer, &adc_data, sizeof(adc_data));
	if(ret)
	{
		printk("ret = %d\n", ret);
	}
	return count;
}

static int hx711_release(struct inode *inode, struct file *filp)
{
	return 0;
}


/* 设备操作函数 */
/* 设备操作函数 */
static struct file_operations newchrled_fops = {
	.owner = THIS_MODULE,
	.open = hx711_open,
	.read = hx711_read,
	.write = hx711_write,
	.release = hx711_release,
};

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init hx711_init(void)
{
	int i;

    /* 初始化HX711 */
    for(i = 0; i < ARRAY_SIZE(led_info); i++) {//遍历led_info结构体
        int result = gpio_request(led_info[i].gpio,led_info[i].name);//先向内核申请GPIO硬件资源;
		if(result!=0){//判断申请的GPIO资源是否成功，失败返回提示信息
			printk(KERN_ERR "GPIO%d has used...led init err!\n",led_info[i].gpio);//打印错误信息
			return -1;
		}		
    }
	gpio_direction_output(led_info[0].gpio, 0);//设置 GPIO 为输出功能，输出0  //sclk
	gpio_direction_input(led_info[1].gpio);//设置 GPIO 为输人功能       //sda	


	/* 注册字符设备驱动 */
	/* 1、创建设备号 */
	if (newchrled.major) {		/*  定义了设备号 */
		newchrled.devid = MKDEV(newchrled.major, 0);
		register_chrdev_region(newchrled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&newchrled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);	/* 申请设备号 */
		newchrled.major = MAJOR(newchrled.devid);	/* 获取分配号的主设备号 */
		newchrled.minor = MINOR(newchrled.devid);	/* 获取分配号的次设备号 */
	}
	printk("newcheled major=%d,minor=%d\r\n",newchrled.major, newchrled.minor);	
	
	/* 2、初始化cdev */
	newchrled.cdev.owner = THIS_MODULE;
	cdev_init(&newchrled.cdev, &newchrled_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&newchrled.cdev, newchrled.devid, NEWCHRLED_CNT);

	/* 4、创建类 */
	newchrled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
	if (IS_ERR(newchrled.class)) {
		return PTR_ERR(newchrled.class);
	}

	/* 5、创建设备 */
	newchrled.device = device_create(newchrled.class, NULL, newchrled.devid, NULL, NEWCHRLED_NAME);
	if (IS_ERR(newchrled.device)) {
		return PTR_ERR(newchrled.device);
	}
	
	printk(KERN_INFO "hx711_drv_init ok...\n");
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit hx711_exit(void)
{
        int i;	
	//释放gpio
        for(i = 0; i < ARRAY_SIZE(led_info); i++) {//遍历led_info结构体
            gpio_free(led_info[i].gpio);//释放GPIO硬件资源
        } 
	/* 注销字符设备驱动 */
	cdev_del(&newchrled.cdev);/*  删除cdev */
	unregister_chrdev_region(newchrled.devid, NEWCHRLED_CNT); /* 注销设备号 */

	device_destroy(newchrled.class, newchrled.devid);
	class_destroy(newchrled.class);
}

module_init(hx711_init);
module_exit(hx711_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lizhouzhou");
