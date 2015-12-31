# Net Gap网闸项目K60 ARM网管Code
	MDK工程：
	\Project\zding_project\NetGap\MDK\templete.uvprojx
# 超核Kinetis 开发包(固件库)

包含Freescale Kinetis K 系列及 KL 系列底层驱动及实例代码
包含RT-Thread实时操作系统（with LWIP 1.4.0）
包含FatFs R0.10a文件系统

#Debug Uart：
	#define BOARD_UART_DEBUG_MAP            UART3_RX_PB10_TX_PB11

#K60 <-> FPGA Communication Uart:
	#define K60FPGA_UartMap UART4_RX_PC14_TX_PC15
	#define K60FPGA_UartPort HW_UART4
	#define K60FPGA_UartBaudrate 9600
	
线程1：
	收发UDP数据包
线程2：
	Heartbeat心跳UDP数据发送
线程3：
	FatFs文件系统读写

-----Log-----

2015.10.30 16:00
	K60 read the config.csv from the TF card, then transfer it to FPGA via UART4_RX_PC14_TX_PC15 at baudrate 9600.
	FPGA can be set by K60 Uart4.
	BUG:
		When repower the PCB board everything is good.
		When push the MASTER RESETN button, K60 net data fails.

2015.11.02 16:57
	K60 can receive multiple configuration parts from the UDP Data, and restore them together as the CFGNET.CSV in TF card

	BUG:
		still need to judge '#' to renew the CFGNET.CSV


2015.11.03 20:03
	Finish the rebuilting of CFGNET.CSV when get the '#' indicates the starting of the configuration file.
	And continuesly write configuration part into CFGNET.CSV without starting '#'
	
	BUG:
		when K60 gets the UDP data larger than 256 Bytes, after receving dozen of parts of this kind of UDP data (larger than 256), the whole system DOWN! which is linked to the SRAM problem.


2015.11.06 13:25
	Change the program struction into Switch&Case mode.
	at the initial time, configure FPGA with config.csv
	input '!' at the beginning of UDP data to configure FPGA with config.csv
	input '$' in the end of the UDP commend to configure FPGA with cfgnet.csv
