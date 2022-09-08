#include "post_thread.h"

void* post(void* thread_arg)
{
        struct thread_data *my_data =  (struct thread_data *) thread_arg;
        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char str1[4096], str2[4096], buf[BUFSIZE], *str;
        socklen_t len;

        //fd_set   t_set1;
        //struct timeval  tv;
 
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("创建网络连接失败,本线程即将终止---socket error!\n");
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("连接到服务器失败,connect error!\n");
                exit(0);
        }
        printf("与远端建立了连接\n");
 

        //FD_ZERO(&t_set1);
        //FD_SET(sockfd, &t_set1);
        
        char last_post_type[10];
        int last_post_weight;

        strcpy(last_post_type, my_data->type);
        last_post_weight = my_data->weight; 
//-------------------------------------------------------------------------
        umask(0);
        //创建管道文件
        int n = mkfifo("./test.fifo",0664 );
        //管道文件不是因为存在而创建失败
        if (n < 0 && errno != EEXIST)
        {
                perror("mkfifo error");
                return (void*)-1;
        }
        //以只读方式获取管道文件的操作句柄
        int fd = open("./test.fifo", O_RDONLY);
        if (fd < 0)
        {
                perror("open fifo error");
                return (void*)-1;
        }
        printf("open fifo success\n");
        //一直读数据
//---------------------------------------------------------------------------
        while(1)
        { 
                int buf[10] = {0};
                int ret_fd = read(fd, buf, 10);
                if (ret_fd < 0)
                {
                        perror("read error");
                        return (void*)-1;
                }
                //所有写端都关闭
                else if (ret_fd == 0)
                {
                        perror("all write closed");
                        return (void*)-1;
                }

                if(buf[0]==1 && buf[1]==1 && buf[2]==1)
                {
                        memset(my_data->type, 0, 20);
                        strcpy(my_data->type, "bananan");
                }
                else if(buf[0]==2 && buf[1]==2 && buf[2]==2)
                {
                        memset(my_data->type, 0, 20);
                        strcpy(my_data->type, "mango");
                }
                else if(buf[0]==3 && buf[1]==3 && buf[2]==3)
                {
                        memset(my_data->type, 0, 20);
                        strcpy(my_data->type, "pear");
                }
                else if(buf[0]==4 && buf[1]==4 && buf[2]==4)
                {
                        memset(my_data->type, 0, 20);
                        strcpy(my_data->type, "nectarinee");
                }
                else if(buf[0]==5 && buf[1]==5 && buf[2]==5)
                {
                        memset(my_data->type, 0, 20);
                        strcpy(my_data->type, "apple");
                }


                if(strcmp(last_post_type,my_data->type) || abs(last_post_weight - my_data->weight) > 10)
                {
                        printf("开始发送消息到服务器............................\n");
                        memset(str2, 0, 4096);
                        sprintf(str2, "type=%s&weight=%d",my_data->type,my_data->weight);
                        str=(char *)malloc(128);
                        len = strlen(str2);
                        sprintf(str, "%d", len);
                
                        memset(str1, 0, 4096);
                        strcat(str1, "POST /api/info HTTP/1.1\n");
                        strcat(str1, "Host: 192.168.40.8\n");
                        strcat(str1, "Content-Type: application/x-www-form-urlencoded\n");
                        strcat(str1, "Content-Length: ");
                        strcat(str1, str);
                        strcat(str1, "\n\n");
                        //str2的值为post的数据
                        strcat(str1, str2);
                        strcat(str1, "\r\n\r\n");
                        printf("str1:%s\n",str1);

                        ret = write(sockfd,str1,strlen(str1));
                        if (ret < 0) {
                                printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
                                exit(0);
                        }else{
                                printf("消息发送成功，共发送了%d个字节！\n\n", ret);
                        }
                }
                strcpy(last_post_type, my_data->type);
                last_post_weight = my_data->weight; 

                sleep(1);
        }
        close(sockfd);
 
 
        return NULL;
}
