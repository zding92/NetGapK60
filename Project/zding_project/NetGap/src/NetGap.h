#include <rtthread.h>
#include <lwip/sockets.h> /* ??BSD socket,????sockets.h??? */
#include "board.h"
#include "components.h"
#include "chlib_k.h"
#include "sram.h"
#include "drv_uart.h"
#include "uart.h"
#include "string.h"
#include <errno.h>
//#include <dfs_posix.h> 
//============FatFs System Files and SD hardware driver================
#include "diskio.h"
#include "ff.h"
#include "sd.h"
//===============================TF Driver=============================
    //#include "drv_TFCard.h"
    #include "drv_sd.h"
//==============================If Using DFS===========================
    #ifdef RT_USING_DFS
    /* ??DFS ???? */
    #include <dfs_fs.h>
    #include <dfs_elm.h>
    #endif
//========Net Defination=======
#define BUFSZ	2048 //BUFSZ is the largest Bytes the UDP server can receive
#define localPort 61019
#define heartBeatPort 61020

#define waitForUDPData 0
#define UDPDataReadReady 1

//===============K60FPGA_Uart Defination==================
//#define K60FPGA_UartMap UART0_RX_PB16_TX_PB17
#define K60FPGA_UartMap UART4_RX_PC14_TX_PC15
#define K60FPGA_UartPort HW_UART4
#define K60FPGA_UartBaudrate 9600
//=============================================
#define FRDM

void configFromTF(const TCHAR* path	/* Pointer to the file name */);

void t4_thread_entry(void* parameter);