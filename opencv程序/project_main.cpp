#include<opencv2/opencv.hpp>
#include <iostream>
#include<cmath>

#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include"fruit.h"

#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace std;
using namespace cv;

#define IPC_KEY 0x12345678

int main()
{
    int weight;
    VideoCapture capture(1);
    int delay = 1000 / 30;
    int j = 0;
    int n = 0;
    string name = "d:/image/fruit/";
    int buf[10] = {0};

    umask(0);
    int ret = mkfifo("./test.fifo", 0664);
    if (ret < 0 && errno != EEXIST)
    {
        perror("mkfifo error");
        return -1;
    }
    //以只写的方式打开管道文件
    int fd = open("./test.fifo", O_WRONLY);
    if (fd < 0)
    {
        perror("open fifo error");
        return -1;
    }
    printf("open fifo success\n");

    int i = 0;
    int index=0;
    myprocess p1;
    int key;
    while (1)
    {   
        buf[10] ={0};
        while(index < 10)
        {
            Mat frame;
            capture >> frame;
            imshow("show", frame);
            //imwrite("camera1.jpg", frame);
            key = waitKey(delay);
            if (27 == (char)key)
            {
                imwrite((name + to_string(j)) + ".jpg", frame);
                j++;
            }
            
             int a = p1.process(frame,10);
            if(1<=a && a<=5)
            {
                buf[index] == a;
                index++;
            }
    
        } 
        cout << "write data success\n" << endl;
        sleep(1);
    // p1.linefill();
    }

    close(fd);
    return 0;
   // Mat src;
   // src = imread("d:/image/fruit/38.jpg");
   
    //  std::cout << "Hello World!\n";
}