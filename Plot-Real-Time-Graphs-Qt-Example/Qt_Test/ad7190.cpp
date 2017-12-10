/*
MIT License
Copyright (c) 2017 DeeplyEmbedded
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 * ad7190.cpp
 *
 *  Created on  : Dec 10, 2017
 *  Author      : Vinay Divakar
 *  Description : This is a Library for communicating with the AD7190 ADC using the BeagleBone Black.
 *  website     : www.deeplyemebedded.org
 */

/* Includes */
#include<QDebug>
#include<unistd.h>
#include<QTimer>
#include<QFile>
#include <unistd.h>
#include "ad7190.h"

/* Debug Utilities */
#define AD7190_DBG

/* Constructor will initialize the AD7190 commands */
AD7190::AD7190()
{
    init_AD7190_cmds();
}

AD7190::~AD7190()
{

}

/****************************************************************
 * Function Name : init_AD7190_reg_cmds
 * Description   : API to initialize the AD7190 commands structure
 *                 for communication
 * Returns       : None
 * Params        @ad7190_reg_ptr: Pointer to commands structure
 ****************************************************************/
void AD7190::init_AD7190_reg_cmds(AD7190_REG_Ptr ad7190_reg_ptr)
{
    ad7190_reg_ptr->cmd_rd_ID_reg = COM_READ_ID_REG_CMD;
    ad7190_reg_ptr->cmd_rd_config_reg = COM_READ_CONFIG_REG_CMD;
    ad7190_reg_ptr->cmd_rd_data_reg = COM_READ_DATA_REG_CMD;
    ad7190_reg_ptr->cmd_rd_full_scale_reg = COM_READ_FULL_SCALE_REG_CMD;
    ad7190_reg_ptr->cmd_rd_gpocon_reg = COM_READ_GPCON_REG_CMD;
    ad7190_reg_ptr->cmd_rd_mode_reg = COM_READ_MODE_REG_CMD;
    ad7190_reg_ptr->cmd_rd_offset_reg = COM_READ_OFFSET_REG_CMD;
    ad7190_reg_ptr->cmd_rd_status_reg = COM_READ_STATUS_REG_CMD;
    ad7190_reg_ptr->cmd_wr_gpocon_reg = COM_WRITE_GPCON_RED_CMD;
    ad7190_reg_ptr->cmd_wr_config_reg = COM_WRITE_CONFIG_REG_CMD;
    ad7190_reg_ptr->cmd_wr_mode_reg = COM_WRITE_MODE_REG_CMD;
}


/****************************************************************
 * Function Name : init_AD7190_cmds
 * Description   : Initialize the AD7190 commands structure
 *                 for communication
 * Returns       : None
 * Params        : None
 ****************************************************************/
void AD7190::init_AD7190_cmds()
{
    init_AD7190_reg_cmds(&ad7190_obj);

#ifdef AD7190_DBG
    qDebug("[init_AD7190_cmds]cmd_rd_ID_reg         = 0X%x", ad7190_obj.cmd_rd_ID_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_config_reg     = 0X%x", ad7190_obj.cmd_rd_config_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_data_reg       = 0X%x", ad7190_obj.cmd_rd_data_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_full_scale_reg = 0X%x", ad7190_obj.cmd_rd_full_scale_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_gpocon_reg     = 0X%x", ad7190_obj.cmd_rd_gpocon_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_mode_reg       = 0X%x", ad7190_obj.cmd_rd_mode_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_offset_reg     = 0X%x", ad7190_obj.cmd_rd_offset_reg);
    qDebug("[init_AD7190_cmds]cmd_rd_status_reg     = 0X%x", ad7190_obj.cmd_rd_status_reg);
    qDebug("[init_AD7190_cmds]cmd_wr_gpocon_reg     = 0X%x", ad7190_obj.cmd_wr_gpocon_reg);
    qDebug("[init_AD7190_cmds]cmd_wr_config_reg     = 0X%x", ad7190_obj.cmd_wr_config_reg);
    qDebug("[init_AD7190_cmds]cmd_wr_mode_reg       = 0X%x", ad7190_obj.cmd_wr_mode_reg);
    qDebug("\r\n");
#endif

}


/****************************************************************
 * Function Name : AD7190_configure
 * Description   : Configure/Write to the configuration register
 *                 for communication
 * Returns       : (-1) typecast for error, 0 for success
 * Params        @cbyte_2: Data to write to Most Significant Byte
 *               @cbyte_1: Data to write to Middle Byte
 *               @cbyte_0: Data to write to Least Significant Byte
 ****************************************************************/
char AD7190::AD7190_configure(unsigned char cbyte_2, unsigned char cbyte_1, unsigned char cbyte_0)
{
    char check_ret = 0x00;
    unsigned char ad7190_id = 0x00;

    ad7190_id = AD7190_get_ID();
    qDebug("Initializing AD7190 ADC    Device ID    = 0x%x", ad7190_id);

#ifdef AD7190_DBG
    qDebug("[AD7190_configure]cmd_wr_config_reg     = 0X%x", ad7190_obj.cmd_wr_config_reg);
#endif

    /* Go to write to configuration register */
    check_ret = SPIDEV1_single_transfer(ad7190_obj.cmd_wr_config_reg);

    /* Write to Most Significant Byte */
    check_ret = SPIDEV1_single_transfer(cbyte_2);

    /* Write to Mid Byte */
    check_ret = SPIDEV1_single_transfer(cbyte_1);

    /* Write to Least Significant Byte */
    check_ret = SPIDEV1_single_transfer(cbyte_0);

    return (check_ret);
}


/****************************************************************
 * Function Name : AD7190_mode
 * Description   : Configure/Write to the mode register
 *                 for communication
 * Returns       : (-1) typecast for error, 0 for success
 * Params        @mbyte_2: Data to write to Most Significant Byte
 *               @mbyte_1: Data to write to Middle Byte
 *               @mbyte_fs: Data to write to Least Significant Byte
 ****************************************************************/
char AD7190::AD7190_mode(unsigned char mbyte_2, unsigned char mbyte_1, unsigned char mbyte_fs)
{
    char check_ret = 0x00;

#ifdef AD7190_DBG
    qDebug("[init_AD7190_cmds]cmd_wr_mode_reg       = 0X%x", ad7190_obj.cmd_wr_mode_reg);
#endif

    /* Go to write to configuration register */
    check_ret = SPIDEV1_single_transfer(ad7190_obj.cmd_wr_mode_reg);

    /* Write to Most Significant Byte */
    check_ret = SPIDEV1_single_transfer(mbyte_2);

    /* Write to Mid Byte */
    check_ret = SPIDEV1_single_transfer(mbyte_1);

    /* Write to Least Significant Byte */
    check_ret = SPIDEV1_single_transfer(mbyte_fs);

    return (check_ret);
}


/****************************************************************
 * Function Name : AD7190_read_status_reg
 * Description   : Read the status register
 * Returns       : Contents of the status register
 * Params        : None
 ****************************************************************/
unsigned char AD7190::AD7190_read_status_reg()
{
    unsigned char status_reg = 0x00;

    SPIDEV1_single_transfer(ad7190_obj.cmd_rd_status_reg);
    status_reg = SPIDEV1_single_transfer(0x00);
    return (status_reg);
}


/****************************************************************
 * Function Name : AD7190_get_ID
 * Description   : Read the ID register
 * Returns       : Contents of the ID register
 * Params        : None
 ****************************************************************/
unsigned char AD7190::AD7190_get_ID()
{
    unsigned char ad7190_ID = 0x00;

    SPIDEV1_single_transfer(ad7190_obj.cmd_rd_ID_reg);
    ad7190_ID = SPIDEV1_single_transfer(0x00);
    return (ad7190_ID);
}


/****************************************************************
 * Function Name : AD7190_reset
 * Description   : Resets the AD7190
 * Returns       : None
 * Params        : None
 ****************************************************************/
void AD7190::AD7190_reset()
{
    char cnt;

    for(cnt = 0 ; cnt < 5 ; cnt++)
        SPIDEV1_single_transfer(0xFF);
}


/****************************************************************
 * Function Name : AD7190_read_data
 * Description   : Read the data register
 * Returns       : Contents of the data register
 * Params        : None
 ****************************************************************/
unsigned long AD7190::AD7190_read_data()
{
    unsigned char Tx_bytes[2], Rx_bytes[2], reg_status = 0, drdy_bit;
    unsigned long adc_data = 0x00;
    memset(Tx_bytes, 0xFF, sizeof(Tx_bytes));
    memset(Rx_bytes, 0, sizeof(Rx_bytes));
    drdy_bit = 1;

    reg_status = AD7190_read_status_reg();
    drdy_bit = reg_status & 0x80;

    if (drdy_bit == 0x00)
    {
        SPIDEV1_single_transfer(ad7190_obj.cmd_rd_data_reg);
        if (SPIDEV1_transfer(Tx_bytes, Rx_bytes, READ_ADC_DATA_SIZE) == 0)
        {
            adc_data = (adc_data | Rx_bytes[0]) << 8;
            adc_data = (adc_data | Rx_bytes[1]) << 8;
            adc_data = adc_data | Rx_bytes[2];
        }
        else
        {
            printf("(AD7190_read_test)Transaction Failed\r\n");
            adc_data = 0x00;
        }
    }
    else
    {
        adc_data = 0x00;
    }
    return (adc_data);
}

/****************************************************************
 * Function Name : AD7190_init_thread
 * Description   : API to initialize and start the AD7190
 * Returns       : None
 * Params        : None
 ****************************************************************/
void AD7190::AD7190_init_thread()
{
    unsigned long adc_raw = 0x00;

    /* Set up the AD7190 */
    AD7190_reset();
    AD7190_configure(CONFIG_REG_CMD_MSB, CONFIG_REG_CMD_MID, CONFIG_REG_CMD_LSB);
    AD7190_mode(MODE_REG_CMD_MSB, MODE_REG_CMD_MID, MODE_REG_CMD_LSB);
    AD7190_dump_regs(DUMP_CONFIG_AND_MODE_REG_CONTENTS);

    while(1)
    {
        adc_raw = 0x00;

        /* Read the digitized data */
        adc_raw = AD7190_read_data();

        /* Check for data validity */
        if(adc_raw != 0x00)
            /* emit the new data to the mainwindow thread */
            emit new_adc_data(adc_raw);
    }
}

/****************************************************************
 * Function Name : AD7190_dump_regs
 * Description   : Dump the contents of the registers
 * Returns       : None
 * Params        @regs_to_dump: Registers to dump
 ****************************************************************/
void AD7190::AD7190_dump_regs(REG_DumpT regs_to_dump)
{
    unsigned char Tx_bytes[2], Rx_bytes[2], reg_status_id = 0;
    unsigned long read_config_mode = 0x00;
    memset(Tx_bytes, 0xFF, sizeof(Tx_bytes));
    memset(Rx_bytes, 0, sizeof(Rx_bytes));
    switch(regs_to_dump)
    {
    case DUMP_CONFIG_REG_CONTENTS:
        SPIDEV1_single_transfer(ad7190_obj.cmd_rd_config_reg);
        if (SPIDEV1_transfer(Tx_bytes, Rx_bytes, 3) == 0)
        {
            read_config_mode = (read_config_mode | Rx_bytes[0]) << 8;
            read_config_mode = (read_config_mode | Rx_bytes[1]) << 8;
            read_config_mode = read_config_mode | Rx_bytes[2];
            qDebug("Dump - Configuration Register  = 0X%x",(unsigned int)read_config_mode);
        }
        else
        {
            qDebug("(AD7190_dump_regs)Transaction Failed\r\n");
        }
        break;
    case DUMP_MODE_REG_CONTENTS:
        SPIDEV1_single_transfer(ad7190_obj.cmd_rd_mode_reg);
        if (SPIDEV1_transfer(Tx_bytes, Rx_bytes, 3) == 0)
        {
            read_config_mode = (read_config_mode | Rx_bytes[0]) << 8;
            read_config_mode = (read_config_mode | Rx_bytes[1]) << 8;
            read_config_mode = read_config_mode | Rx_bytes[2];
            qDebug("Dump - Mode Register  = 0X%x",(unsigned int)read_config_mode);
        }
        else
        {
            qDebug("(AD7190_dump_regs)Transaction Failed\r\n");
        }
        break;
    case DUMP_STATUS_REG_CONTENTS:
        reg_status_id = AD7190_read_status_reg();
        qDebug("Dump - Status Register  = 0X%x",reg_status_id);
        break;
    case DUMP_ID_REG_CONTENTS:
        reg_status_id = AD7190_get_ID();
        qDebug("Dump - ID Register  = 0X%x",reg_status_id);
        break;
    case DUMP_CONFIG_AND_MODE_REG_CONTENTS:
    default:
        SPIDEV1_single_transfer(ad7190_obj.cmd_rd_config_reg);
        if (SPIDEV1_transfer(Tx_bytes, Rx_bytes, 3) == 0)
        {
            read_config_mode = (read_config_mode | Rx_bytes[0]) << 8;
            read_config_mode = (read_config_mode | Rx_bytes[1]) << 8;
            read_config_mode = read_config_mode | Rx_bytes[2];
            qDebug("Dump - Configuration Register  = 0X%x",(unsigned int)read_config_mode);
        }
        else
        {
            qDebug("(AD7190_dump_regs)Transaction Failed\r\n");
        }

        memset(Rx_bytes, 0, sizeof(Rx_bytes));
        read_config_mode = 0x00;

        SPIDEV1_single_transfer(ad7190_obj.cmd_rd_mode_reg);
        if (SPIDEV1_transfer(Tx_bytes, Rx_bytes, 3) == 0)
        {
            read_config_mode = (read_config_mode | Rx_bytes[0]) << 8;
            read_config_mode = (read_config_mode | Rx_bytes[1]) << 8;
            read_config_mode = read_config_mode | Rx_bytes[2];
            qDebug("Dump - Mode Register           = 0X%x",(unsigned int)read_config_mode);
        }
        else
        {
            qDebug("(AD7190_dump_regs)Transaction Failed\r\n");
        }
        break;
    }
}
