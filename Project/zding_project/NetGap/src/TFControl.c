#include <rtthread.h>
#include <lwip/sockets.h> /* ??BSD socket,????sockets.h??? */
//#include "board.h"
#include "string.h"
#include "uart.h"
//======????======
#include "diskio.h"
#include "ff.h"
//====================
/* Ð´TXTÎÄ¼þ */
FRESULT write_txt(char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    FIL file;
    char *fn;   /* This function is assuming non-Unicode cfg. */
    unsigned char writebuff[]="zding txt test\r\n";
    unsigned int ByteWritten;
    res=f_open(&file,path,FA_OPEN_ALWAYS|FA_WRITE);
    UART_printf("f_open_error%d",res);
//    if (res==FR_OK) 
//    {
//        UART_printf("txt file successfully built or found\r\n");
//        res=f_write(&file,writebuff,sizeof(writebuff),&ByteWritten);
//        if (res==FR_OK)
//            UART_printf("txt file successfully written\r\n");
//        else UART_printf("txt file written failed for reason of %d\r\n",res);
//    }
//    else UART_printf("txt file open failed for reason of %d\r\n",res);
    f_close(&file);
    return res;
}