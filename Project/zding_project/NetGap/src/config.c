#include <NetGap.h>

/**
*"configFromTF" is used to config FPGA via K60FPGA_UartPort, the config file name is path
*parameter: @ path is the file name to be configured
*/
void configFromTF(const TCHAR* path	/* Pointer to the file name */){
    FRESULT res;
    FILINFO fno;
    DIR dir;
    FIL file;
    char *fn;   /* This function is assuming non-Unicode cfg. */

    char readbuff[50];
    unsigned int ByteWritten;
    unsigned int ByteRead;
    
    unsigned char readBuffPointer;//readbuff?????
    unsigned char readBuffPartSum = 0;//readbuff????????????
    unsigned char commerCnt = 0;
    res=f_open(&file,path,FA_OPEN_EXISTING|FA_READ);
                if (res==FR_OK) 
                {
               //===========================Read File===================================
                    UART_printf("txt file successfully built or found\r\n");
                    while( !f_eof(&file) )
                    {
                        f_gets(readbuff,50,&file);
                        readBuffPointer = 0;
                        readBuffPartSum = 0;
                        while(readbuff[readBuffPointer]!='\n'){
                            if (readbuff[readBuffPointer] == ','){
                                UART_WriteByte(K60FPGA_UartPort, readBuffPartSum);
                                //UART_printf("File Read:%x\r\n",readBuffPartSum);
                                readBuffPartSum = 0;

                            }
                            else{
                                readBuffPartSum = readBuffPartSum*10+(readbuff[readBuffPointer]-0x30);
                            }
                            readBuffPointer++;
                        }
                        UART_printf("File Read:%s\r\n",readbuff);
                        DelayMs(5);         

                    }
                    //UART_printf("File Read:%s\r\n",readbuff);
                }
                else UART_printf("txt file open failed for reason of %d\r\n",res);
                f_close(&file);
}



