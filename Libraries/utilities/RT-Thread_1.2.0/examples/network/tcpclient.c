#include <rtthread.h>
#include <lwip/netdb.h> /* 为了解析主机名，需要包含netdb.h头文件 */
#include <lwip/sockets.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include "gpio.h"
#include "uart.h"
#include "sd.h"
#include "ff.h"
#include "gui.h"
#include <string.h>
#include <stdio.h>
#define BUFSZ	65535 /*网络数据缓存段大小*/

static const char send_data[] = "This is TCP Client from RT-Thread."; /* 发送用到的数据 */
static FRESULT write_txt(char* path,char* writebuff,int size_of_file);
void tcpclient(const char* url, int port)
{
    char *recv_data;
    struct hostent *host;
    int sock, bytes_received;
    struct sockaddr_in server_addr;

    /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析） */
    host = gethostbyname(url);

    /* 分配用于存放接收数据的缓冲 */
    recv_data = rt_malloc(BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    /* 创建一个socket，类型是SOCKET_STREAM，TCP类型 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* 创建socket失败 */
        rt_kprintf("Socket error\n");

        /* 释放接收缓冲 */
        rt_free(recv_data);
        return;
    }

    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    /* 连接到服务端 */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 连接失败 */
        rt_kprintf("Connect fail!\n");
        lwip_close(sock);

        /*释放接收缓冲 */
        rt_free(recv_data);
        return;
    }

    rt_kprintf("connection ok\r\n");
    
    /*SD卡初始化*/
    SD_QuickInit(10*1000*1000);
    printf("SD size:%dMB\r\n", SD_GetSizeInMB());
    FATFS fs_sd;
    FATFS *fs = &fs_sd;
    /*挂载文件系统*/
    f_mount(fs, "0:", 0);
    
    while(1)
    {
        /* 从sock连接中接收最大BUFSZ - 1字节数据 */
        bytes_received = recv(sock, recv_data, BUFSZ - 1, 0);
        if (bytes_received <= 0)
        {
            /* 接收失败，关闭这个连接 */
            lwip_close(sock);

            /* 释放接收缓冲 */
            rt_free(recv_data);
            break;
        }

        /* 有接收到数据，把末端清零 */
        recv_data[bytes_received] = '\0';

        if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
        {
            /* 如果是首字母是q或Q，关闭这个连接 */
            lwip_close(sock);

            /* 释放接收缓冲 */
            rt_free(recv_data);
            break;
        }
        else
        {
            /* 在控制终端显示收到的数据 */
            rt_kprintf("\nRecieved data = %s " , recv_data);
            rt_kprintf("The received byte = %d\r\n",bytes_received);
            //rt_kprintf("The received byte = %d\r\n",sizeof(recv_data));
            write_txt("0:ZDING.txt",recv_data,bytes_received);
        }

        /* 发送数据到sock连接 */
        send(sock,send_data,strlen(send_data), 0);
    }

    return;
}
/*write txt file*/
static FRESULT write_txt(char* path,char* writebuff,int size_of_file)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    FIL file;
    char *fn;   /* This function is assuming non-Unicode cfg. */
    //unsigned char writebuff1[]="zding net txt test_with xdw\r\n";
    unsigned int ByteWritten;
    res=f_open(&file,path,FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if (res==FR_OK) 
    {
        printf("txt file successfully built or found\r\n");
        res=f_write(&file,writebuff,size_of_file,&ByteWritten);
        if (res==FR_OK)
            printf("txt file successfully written\r\n");
        else printf("txt file written failed for reason of %d\r\n",res);
    }
    else printf("txt file open failed for reason of %d\r\n",res);
    f_close(&file);
    return res;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
/* 输出tcpclient函数到finsh shell中 */
FINSH_FUNCTION_EXPORT(tcpclient, startup tcp client);
#endif
