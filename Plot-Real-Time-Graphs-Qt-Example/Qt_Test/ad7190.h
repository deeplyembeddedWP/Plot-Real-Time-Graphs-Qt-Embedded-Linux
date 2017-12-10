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
 * ad7190.h
 *
 *  Created on  : Dec 10, 2017
 *  Author      : Vinay Divakar
 *  Description : This is a Library for communicating with the AD7190 ADC using the BeagleBone Black.
 *  website     : www.deeplyemebedded.org
 */

#ifndef AD7190_H
#define AD7190_H

/* Includes */
#include <QObject>

/* To inherit from SPI */
#include "spi.h"

/* Commands to write to specific registers */
#define COM_WRITE_CONFIG_REG_CMD                  0x10
#define COM_WRITE_MODE_REG_CMD                    0x08
#define COM_WRITE_GPCON_RED_CMD                   0x28

/* Commands to read from specific registers */
#define COM_READ_CONFIG_REG_CMD                   0x50
#define COM_READ_STATUS_REG_CMD                   0x40
#define COM_READ_MODE_REG_CMD                     0x48
#define COM_READ_DATA_REG_CMD                     0x58
#define COM_READ_GPCON_REG_CMD                    0x68
#define COM_READ_ID_REG_CMD                       0x60
#define COM_READ_OFFSET_REG_CMD                   0x70
#define COM_READ_FULL_SCALE_REG_CMD               0x78

/* Sampling Rates */
#define FS_100_HZ                                 0x30
#define FS_1200_HZ                                0x04
#define FS_960_HZ                                 0x05
#define FS_2400_HZ                                0x02

/* Register settings commands for Configuration Register */
#define CONFIG_REG_CMD_MSB                        0x08 //Gain = 1 and Uni-Polar(0x08)
#define CONFIG_REG_CMD_MID                        0x02

/*May have to change Gain depending on input signal voltage
See Table 19 in AD7190 datasheet for more information*/
#define CONFIG_REG_CMD_LSB                        0x00

/* Register settings commands for Mode Register */
#define MODE_REG_CMD_MSB                          0x08
#define MODE_REG_CMD_MID                          0x00
#define MODE_REG_CMD_LSB                          FS_100_HZ

/* Read the data register continously and place the data on DOUT */
#define COMM_REG_CREAD                            0x5C

/* Generic definitions can be added here */
#define READ_ADC_DATA_SIZE                        3


/* Structure Map of AD7190 internal registers
 * for read and write operations
 */
typedef struct{
    unsigned char cmd_rd_status_reg;
    unsigned char cmd_wr_mode_reg;
    unsigned char cmd_rd_mode_reg;
    unsigned char cmd_wr_config_reg;
    unsigned char cmd_rd_config_reg;
    unsigned char cmd_rd_data_reg;
    unsigned char cmd_rd_ID_reg;
    unsigned char cmd_rd_gpocon_reg;
    unsigned char cmd_wr_gpocon_reg;
    unsigned char cmd_rd_offset_reg;
    unsigned char cmd_rd_full_scale_reg;
}AD7190_REG_T, *AD7190_REG_Ptr;

/* Enum commands to dump register contents */
typedef enum{
    DUMP_CONFIG_REG_CONTENTS,
    DUMP_MODE_REG_CONTENTS,
    DUMP_STATUS_REG_CONTENTS,
    DUMP_ID_REG_CONTENTS,
    DUMP_CONFIG_AND_MODE_REG_CONTENTS
}REG_DumpT;

class AD7190 : public SPI
{
    Q_OBJECT
private:
    AD7190_REG_T ad7190_obj;
    void init_AD7190_reg_cmds(AD7190_REG_Ptr ad7190_reg_ptr);
    void init_AD7190_cmds();

public:
    AD7190();
    ~AD7190();
    char AD7190_configure(unsigned char cbyte_2, unsigned char cbyte_1, unsigned char cbyte_0);
    char AD7190_mode(unsigned char mbyte_2, unsigned char mbyte_1, unsigned char mbyte_fs);
    void AD7190_reset();
    unsigned char AD7190_get_ID();
    unsigned char AD7190_read_status_reg();
    void AD7190_dump_regs(REG_DumpT regs_to_dump);
    unsigned long AD7190_read_data();

signals:
    void new_adc_data(unsigned long adc_data);

public slots:
    void AD7190_init_thread();

};

#endif // AD7190_H
