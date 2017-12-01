/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  vc_i2c_io_interface.c
**	
**
** 
*******************************************************************************/
#include <windows.h>
#include <ceddk.h>
#include "i2c.h"

HANDLE g_hI2cDevice = NULL;
#define HEAD_SIZE	0

/******************************************************************************
**
**	I2c 初始化
**
*******************************************************************************/
int VCI2CInit(void)
{
	RETAILMSG(1,(TEXT("[XF_VCI2CInit] Enter!\r\n")));
	if(g_hI2cDevice == NULL)
	{
		g_hI2cDevice = _I2C_Open();
		if(g_hI2cDevice == NULL || g_hI2cDevice == INVALID_HANDLE_VALUE )
		{
			RETAILMSG(1,(TEXT("[XF_VCI2CInit] Open I2c Fail!\r\n")));
			return FALSE;
		}
	}
	else
	{
		RETAILMSG(1,(TEXT("[XF_VCI2CInit] I2c had open!\r\n")));
	}
	return TRUE;
}

/*------------------------------------------------------------------------------
Function:   VCI2CWrite

Purpose:    用户实现的接口函数,功能为向I2C总线按照固定的时序写数据,时序如下图：

|START|0x8E(8bit)|ACK|register_address高8位|ACK|register_address低8位|ACK|buffer[0]|ACK|buffer[1]|ACK|...|buffer[size-1]|ACK|STOP|

Inputs:     register_address	- 16bit的控制地址 
buffer			- 写入I2C总线数据的存储地址
size				- 写入I2C总线数据的长度, 1 for Byte

Outputs:	  return			- succeed : 1; 
failed  : 0;
------------------------------------------------------------------------------*/
int VCI2CWrite(unsigned char device, unsigned char register_address, unsigned char *buffer, int size)
{
	unsigned char tempdata[100];
	UINT32 u4ClkDiv = 0x64;
	I2C_OPCMD rI2C_OPCMD ={0};

	_I2C_SetClock(g_hI2cDevice,u4ClkDiv);

	tempdata[0] = register_address;
	memcpy(tempdata+1,buffer,size);

	rI2C_OPCMD.dwopcmd = I2C_CMD_SEND_START|I2C_CMD_SEND_DEVADDR_W|I2C_CMD_WRITE_DATA|I2C_CMD_SEND_STOP;
	rI2C_OPCMD.u1DevId = device>>1;
	rI2C_OPCMD.pInBuff = tempdata;
	rI2C_OPCMD.u4InBuffSize = size+1;
	return _I2C_CMD_Operation(g_hI2cDevice, &rI2C_OPCMD);
}

/*------------------------------------------------------------------------------
Function:   VCI2CRead

Purpose:     用户实现的接口函数,功能为从I2C总线按照固定的时序读数据,时序如下图：

|START|0x8E(8bit)|ACK|register_address高8位|ACK|register_address低8位|ACK|START|0x8F(8bit)|ACK|register_address高8位|ACK|register_address低8位|ACK|buffer[0]|ACK|buffer[1]|ACK|...|buffer[size-1]|ACK|STOP|

Inputs:     register_address	- 16bit的控制地址
buffer			- 从I2C总线读取数据的存储地址
size				- 从I2C总线读取数据的长度, 1 for Byte

Outputs:	  return			- succeed : 1; 
failed  : 0;
------------------------------------------------------------------------------*/
int VCI2CRead(unsigned char device, unsigned char register_address, unsigned char*buffer, int size)
{
	int i;

	UINT32 u4ClkDiv = 0x64;
	I2C_OPCMD rI2C_OPCMD ={0};


	_I2C_SetClock(g_hI2cDevice,u4ClkDiv);

	rI2C_OPCMD.dwopcmd = I2C_CMD_SEND_START|I2C_CMD_SEND_DEVADDR_W|I2C_CMD_WRITE_DATA;
	rI2C_OPCMD.u1DevId = device>>1;
	rI2C_OPCMD.pInBuff = &register_address;
	rI2C_OPCMD.u4InBuffSize = 1;
	_I2C_CMD_Operation(g_hI2cDevice, &rI2C_OPCMD);

	rI2C_OPCMD.dwopcmd = I2C_CMD_SEND_START|I2C_CMD_SEND_DEVADDR_R|I2C_CMD_READ_DATA|I2C_CMD_SEND_STOP;
	rI2C_OPCMD.u1DevId = device>>1;
	rI2C_OPCMD.pOutBuff = buffer;
	rI2C_OPCMD.u4OutBuffSize = size;
	return _I2C_CMD_Operation(g_hI2cDevice, &rI2C_OPCMD);
}

/*------------------------------------------------------------------------------
Function:   VCI2CSleep

Purpose:    The I2C driver layer interface which user need to develop.This function
just call the sleep fun on host. 

Inputs:     intreval_ms		- time for sleep, 1 for 1 millisecond 

Outputs:	  return			- void
------------------------------------------------------------------------------*/
void VCI2CSleep (int intreval_ms)
{
	Sleep(intreval_ms);
}

/******************************************************************************
**
**	I2c 反初始化
**
*******************************************************************************/
void VCI2CDenit(void)
{
	RETAILMSG(1,(TEXT("[XF_VCI2CDenit] Enter!\r\n")));
	if(g_hI2cDevice)
	{
		_I2C_Close(g_hI2cDevice);
		g_hI2cDevice = NULL;
	}   
}