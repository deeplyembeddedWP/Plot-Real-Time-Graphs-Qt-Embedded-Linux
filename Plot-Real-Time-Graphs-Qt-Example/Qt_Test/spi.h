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
* spi.h
*
*  Created on  : Nov 12, 2017
*  Author      : Vinay Divakar
*  website     : www.deeplyemebedded.org
*/

#ifndef SPI_H
#define SPI_H

/* Includes */
#include <QObject>

/* Generic Definitions */
#define SPIDEV_BYTES_NUM                 8
#define SPIDEV_DATA_BITS_NUM             8
#define SPIDEV_DELAY_US	                 0
#define SPI_SS_HIGH                      1
#define SPI_SS_LOW                       0
#define SPI_ONE_BYTE                     1

/* No. of bytes per transaction */
#define NO_OF_BYTES                      2

/*Definitions specific to spidev1.0 */
#define SPIDEV1_PATH                     "/dev/spidev1.0"
#define SPIDEV1_BUS_SPEED_HZ             1000000


/* Enum SPI Modes*/
typedef enum{
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
}SPI_MODE;

/*SPI device configuration structure*/
typedef struct{
    char* spi_dev_path;
    int fd_spi;
    unsigned long spi_bytes_num;
    unsigned long spi_bus_speedHZ;
    unsigned char ss_change;
    unsigned short spi_delay_us;
    unsigned char spi_data_bits_No;
    unsigned char spi_mode;
}SPI_DeviceT, *SPI_DevicePtr;

class SPI : public QObject
{
    Q_OBJECT
public:
    explicit SPI(QObject *parent = NULL);
    ~SPI();

    /* SPI API's*/
    int Open_device(char *spi_dev_path, int *fd);
    int Set_SPI_mode(int fd, unsigned char spi_mode);
    int Set_SPI_bits(int fd, unsigned char bits_per_word);
    int Set_SPI_speed(int fd, unsigned long bus_speed_HZ);
    void SPI_Config_init(unsigned long spi_bytes_no, unsigned long spi_bus_speed,
                         unsigned char chip_select, unsigned short spi_delay,
                         unsigned char spi_bits_No, unsigned char mode_spi, SPI_DevicePtr SPI_devicePtr);

    /* API's to initialize and use spidev1.0 - Configure the Bus as you like*/
    int SPI_DEV1_init(unsigned long spi_bytes_no, unsigned long spi_bus_speed,unsigned char chip_select, unsigned short spi_delay,
                      unsigned char spi_bits_No, unsigned char mode_spi);
    int SPIDEV1_transfer(unsigned char *send, unsigned char *receive, unsigned char bytes_num);
    unsigned char SPIDEV1_single_transfer(unsigned char data_byte);

private:

signals:

public slots:
};

#endif // SPI_H
