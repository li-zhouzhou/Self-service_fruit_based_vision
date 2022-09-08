#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define IPC_KEY 0x12345678
int main()
{
    umask(0);
    //创建管道文件
    int ret = mkfifo("./test.fifo",0664 );
    //管道文件不是因为存在而创建失败
    if (ret < 0 && errno != EEXIST)
    {
        perror("mkfifo error");
        return -1;
    }
    //以只读方式获取管道文件的操作句柄
    int fd = open("./test.fifo", O_RDONLY);
    if (fd < 0)
    {
        perror("open fifo error");
        return -1;
    }
    printf("open fifo success\n");
    //一直读数据
    while(1)
    {
        int buf[10] = {0};
        //将从管道读取的文件写到buf中
        int ret = read(fd, buf, 10);
        if (ret < 0)
        {
            perror("read error");
            return -1;
        }
        //所有写端都关闭
        else if (ret == 0)
        {
            perror("all write closed");
            return -1;
        }
        //打印所读到的数据

        for(int i=0;i<10;i++)
        {
            printf("read buf[%d]: %d\n",i, buf[i]);
        }

    }
    close(fd);
    return 0;
}
