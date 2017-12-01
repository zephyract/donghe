/********************************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC). 
************************************************************************************************/

/****************************************************************************
*
*Filename:       i2c.h
*
*Project:          mt33xx
*
*Description:    I2C driver header file
*
*Author:           mtk40499
*
*Time:              11/16/2011
*
*****************************************************************************/

#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//
//  Define:  I2C_DEVICE_NAME
//
#define I2C_DEVICE_NAME         L"I2C1:"



// Transfer buffer size when not use DMA
#define I2C_TRANSFER_BUFFER_SIZE        16     

#define MAX_FS_SPEED 400
#define MAX_HS_SPEED 3400
#define I2C_READ_BIT        0x01
#define I2C_WRITE_BIT	    0x00
//------------------------------------------------------------------------------
typedef enum {
    I2C_ERR_SUCCESS = 0,
    I2C_ERR_INVALID_OPCODE,
    I2C_ERR_INVALID_LENGTH,
    I2C_ERR_INVALID_ADDRESS,
    I2C_ERR_INVALID_TRANSNUM,
    I2C_ERR_INVALID_FS_SPEED,
    I2C_ERR_INVALID_HS_SPEED,
    I2C_ERR_INVALID_POINTER,
    I2C_ERR_NOTSUPPORTED,
    I2C_ERR_ACKERR,
    I2C_ERR_TIMEOUT,
} I2C_ERR_CODE;

//------------------------------------------------------------------------------
typedef enum {
    I2C_OP_READ = 1,
    I2C_OP_WRITE,
    I2C_OP_WRITE_READ
} I2C_TRANSFER_OP;

typedef enum
{
    I2C_TRANSFER_FAST_MODE = 0,
    I2C_TRANSFER_HIGH_SPEED_MODE,
    I2C_TRANSFER_MODE_NUM
}I2C_TRANSFER_MODE;

typedef enum {
    I2C_TRANSFER_STOP = 0,
    I2C_TRANSFER_REPEAT_START
} I2C_TRANSFER_ST_RS;

typedef enum {
    I2C_CLKEXT_DISABLE = 0,
    I2C_CLKEXT_ENABLE
}I2C_CLK;

typedef enum {
    I2C_VAL_TIMEOUT,
    I2C_VAL_CLK_EXT,
    I2C_VAL_DELAY_LEN,
    I2C_VAL_FS_SAMPLE_DIV,
    I2C_VAL_FS_STEP_DIV,
    I2C_VAL_HS_SAMPLE_DIV,
    I2C_VAL_HS_STEP_DIV
}I2C_TRANSFER_PARAM;

typedef enum
{
    I2C_TRANSACTION_FAST_MODE,
    I2C_TRANSACTION_HIGH_SPEED_MODE
} I2C_TRANSACTION_MODE;

typedef enum {
    I2C1_MASTER= 0,
    I2C2_SLAVE,
    I2C1_SLAVE
} I2C_MODE;
#define I2C_TRASFER_LEN (0xff)

typedef enum
{ 
    SIF_0_BIT,
    SIF_8_BIT,
    SIF_16_BIT,
}   SIF_BIT_T;


typedef struct 
{
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT8 *pBuffer;
    UINT8 transfer_len;
    UCHAR ucDev;
    SIF_BIT_T ucAddrType;
    I2C_MODE i2cMode;

}SIF_CONFIG,*PSIF_CONFIG;

//------------------------------------------------------------------------------
//
//  Type:  DEVICE_IFC_I2C
//
//  This structure is used to obtain I2C interface funtion pointers used for
//  in-process calls via IOCTL_DDK_GET_DRIVER_IFC.
//

typedef DWORD (*pI2cFnRead)(HANDLE, PUCHAR, DWORD, LPDWORD);
typedef DWORD (*pI2cFnWrite)(HANDLE, PUCHAR, DWORD, LPDWORD);
typedef BOOL (*pI2cFnIO)(HANDLE, DWORD, UCHAR*, DWORD, UCHAR*, DWORD, DWORD*);

typedef struct {
    HANDLE openContext;
    pI2cFnRead pRead;
    pI2cFnWrite pWrite;
    pI2cFnIO pIO;
} DEVICE_IFC_I2C;

//------------------------------------------------------------------------------
//
//  Type:  DEVICE_CONTEXT_I2C
//
//  This structure is used to store I2C device context.
//
typedef struct {
    DEVICE_IFC_I2C ifc;
    HANDLE hDevice;
} DEVICE_CONTEXT_I2C;

#define DEFAULT_SIF_CLK_DIV             0x64
//------------------------------------------------------------------------------

#define IOCTL_I2C_RAN_ADDR_READ           \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0201, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_RAN_ADDR_WRITE           \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x202, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_CUR_ADDR_READ          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x203, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_RAN_ADDR_WRITEENDWITHOUTSTOP          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x204, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_SEG_CMD          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x205, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_SET_CLOCK          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x206, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C_RAN_ADDR_READ_NORNAL          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x207, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_I2C__CMD_OPERATION          \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x208, METHOD_BUFFERED, FILE_ANY_ACCESS)

enum
{
	I2C_CMD_SEND_START = 1<<0,
	I2C_CMD_SEND_DEVADDR_R = 1<<1,
	I2C_CMD_SEND_DEVADDR_W = 1<<2,
	I2C_CMD_WRITE_DATA = 1<<3,
	I2C_CMD_READ_DATA = 1<<4,
	I2C_CMD_SEND_STOP = 1<<5,
	I2C_MASK_W_NNEED_ACK = 1<<6,//no use
	I2C_MASK_R_NNEED_NACK = 1<<7,
};

typedef struct _I2C_OPCMD
{
	UINT32 dwopcmd;
	UINT8 u1DevId;
	UINT8 *pInBuff;
	UINT32 u4InBuffSize;
    UINT8 *pOutBuff;
	UINT32 u4OutBuffSize;

}I2C_OPCMD;

typedef struct _SEG_CMD
{
    BYTE uSegCmd;
    BYTE uData;
}SEG_CMD;
//------------------------------------------------------------------------------
__inline HANDLE _I2C_Open()
{
    HANDLE hDevice;

    hDevice = CreateFile(I2C_DEVICE_NAME, 0, 0, NULL, 0, 0, NULL);
    
    if (INVALID_HANDLE_VALUE == hDevice)
    {
        RETAILMSG(1, (L"ERROR: Failed to open '%s'\r\n", I2C_DEVICE_NAME));
        return NULL;
    }
    
    return hDevice;
}

__inline VOID _I2C_Close(HANDLE hDevice)
{
    CloseHandle(hDevice);
}

__inline BOOL _I2C_RanAddrRead
    (HANDLE hDevice, SIF_CONFIG *pSifCofig,BYTE *pOutBuffer,
    DWORD outSize)
{
    if (!DeviceIoControl(
            hDevice, IOCTL_I2C_RAN_ADDR_READ, (VOID*)pSifCofig, 
            sizeof(SIF_CONFIG), pOutBuffer, outSize, NULL, NULL))
    {
        //CloseHandle(hDevice);

        RETAILMSG(1, (L"ERROR: _I2C_RanAddrRead Err = %d\r\n",GetLastError()));

        ASSERT(0);

        return FALSE;
    }
    return TRUE;
}

__inline BOOL _I2C_RanAddrWrite
    (HANDLE hDevice, SIF_CONFIG *pSifCofig)
{
    if (!DeviceIoControl(
            hDevice, IOCTL_I2C_RAN_ADDR_WRITE, (VOID*)pSifCofig, 
            sizeof(SIF_CONFIG), NULL, 0, NULL, NULL))
    {
        //CloseHandle(hDevice);

        RETAILMSG(1, (L"ERROR: _I2C_RanAddrWrite Err = %d\r\n",GetLastError()));

        ASSERT(0);

        return FALSE;
    }
    return TRUE;
}

__inline BOOL _I2C_RanAddrWriteEndWithoutStop
    (HANDLE hDevice, SIF_CONFIG *pSifCofig)
{
    if (!DeviceIoControl(
            hDevice, IOCTL_I2C_RAN_ADDR_WRITEENDWITHOUTSTOP, (VOID*)pSifCofig, 
            sizeof(SIF_CONFIG), NULL, 0, NULL, NULL))
    {
        //CloseHandle(hDevice);

        RETAILMSG(1, (L"ERROR: _I2C_RanAddrWrite Err = %d\r\n",GetLastError()));

        ASSERT(0);

        return FALSE;
    }
    return TRUE;
}


__inline BOOL _I2C_CurAddrRead
    (HANDLE hDevice, SIF_CONFIG *pSifCofig,BYTE *pOutBuffer,
    DWORD outSize)
{
    if (!DeviceIoControl(
            hDevice, IOCTL_I2C_CUR_ADDR_READ, (VOID*)pSifCofig, 
            sizeof(SIF_CONFIG), pOutBuffer, outSize, NULL, NULL))
    {
        //CloseHandle(hDevice);

        RETAILMSG(1, (L"ERROR: _I2C_CurAddrRead\r\n"));

        ASSERT(0);

        return FALSE;
    }
    return TRUE;
}

__inline BOOL _I2C_SetClock(HANDLE hDevice, DWORD u4ClkDiv)
{
    if (!DeviceIoControl(
        hDevice, IOCTL_I2C_SET_CLOCK, (VOID*)&u4ClkDiv, 
        sizeof(DWORD), NULL, 0, NULL, NULL))
    {

        RETAILMSG(1, (L"ERROR: _I2C_SetClock ERROR:%d\r\n", GetLastError()));
        ASSERT(0);

        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
_inline BOOL _I2C_CMD_Operation(HANDLE hDevice, I2C_OPCMD *pI2cOpcmd)
{
  if (!DeviceIoControl(
            hDevice, IOCTL_I2C__CMD_OPERATION, (VOID*)pI2cOpcmd, 
            sizeof(pI2cOpcmd), NULL, 0, NULL, NULL))
    {
        //CloseHandle(hDevice);

        RETAILMSG(1, (L"ERROR: _I2C_CMD_Operation Err = %d\r\n",GetLastError()));

//        ASSERT(0);

        return FALSE;
    }
    return TRUE;

}
#ifdef __cplusplus
}
#endif

#endif
