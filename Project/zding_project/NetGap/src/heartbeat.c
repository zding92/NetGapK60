#include <rtthread.h>
#include <lwip/sockets.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include "uart.h"
#define BUFSZ	1024
#define localPort 61019
extern char UDPsend_data[];
void heartbeat()
{
   int sock;
   int bytes_read;
   char *recv_data;
   rt_uint32_t addr_len;
   struct sockaddr_in server_addr, client_addr;

    UART_printf("Enter HeartBeat\n");
   /* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
       //rt_kprintf("Socket error\n");
       UART_printf("Socket error\n");
       return;
   }
   UART_printf("HeartBeat Socket built\n");

   /* 初始化服务端地址 */
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(localPort);
   server_addr.sin_addr.s_addr = INADDR_ANY;
   rt_memset(&(server_addr.sin_zero),0, sizeof(server_addr.sin_zero));

   /* 绑定socket到服务端地址 */
   if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
   {
       /* 绑定地址失败 */
       //rt_kprintf("Bind error\n");
       UART_printf("Bind error\n");
       /* 释放接收用的数据缓冲 */
       rt_free(recv_data);
       return;
   }

   addr_len = sizeof(struct sockaddr);
   sendto(sock, UDPsend_data, strlen(UDPsend_data), 0,
              (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
   
   return;
}

