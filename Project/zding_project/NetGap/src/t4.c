#include <netGap.h>

void t4_thread_entry(void* parameter){
    uint16_t byteReceived; //申请变量，存储接收的数据
    DelayMs(4500); 
    UART_printf("Thread 4 entered\r\n");
    while(1){
        //UART_printf("T4\r\n");
        if (UART_ReadByte(HW_UART3, &byteReceived) == 0){
            //UART_printf("get uart data\r\n");
            UART_printf("get:%c\r\n",(char)byteReceived);
        }
        //DelayMs(2000);
    }
}