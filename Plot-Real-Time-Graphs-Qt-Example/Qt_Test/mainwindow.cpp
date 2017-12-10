/*
* mainwindow.cpp
*
*  Created on  : Dec 10, 2017
*  Author      : Vinay Divakar
*  website     : www.deeplyemebedded.org
*/

#include<QDebug>
#include<QThread>
#include<QFont>
#include<QWSServer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ad7190.h"

/* Global objects */
SPI spi_device1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QWSServer::setCursorVisible(false);
    this->setStyleSheet("background-color: white;");

    /* Initialize SPI */
    if (spi_device1.SPI_DEV1_init(SPIDEV_BYTES_NUM, SPIDEV1_BUS_SPEED_HZ, SPI_SS_LOW,
                                  SPIDEV_DELAY_US, SPIDEV_DATA_BITS_NUM,
                                  SPI_MODE3) == -1)
        qDebug()<<"(Main)spidev1.0 initialization failed\r\n";

    else
        qDebug()<<"(Main)spidev1.0 initialized - READY\r\n";

    /* Configure font settings for Label */
    QFont font;
    font.setPointSize(21);
    font.setItalic(true);
    font.setBold(true);
    ui->label_adc_data->setFont(font);
    ui->label_adc_data->setAlignment(Qt::AlignCenter);
    ui->label_adc_data->setStyleSheet("background-color : white; color : green;");

    /* Configure setup for Thank You Label */
    ui->label_ty->setAlignment(Qt::AlignCenter);
    ui->label_ty->setStyleSheet("background-color : white; color : black;");

    /* Align the intro label appropriately */
    ui->label_intro->setAlignment(Qt::AlignCenter);
    ui->label_intro->setStyleSheet("background-color : white; color : black;");

    /* Thread to continously sample and read the data */
    QThread *thread = new QThread;
    AD7190 *ad7190_dev = new AD7190();
    ad7190_dev->moveToThread(thread);
    connect(thread,SIGNAL(started()),ad7190_dev,SLOT(AD7190_init_thread()));
    connect(ad7190_dev, SIGNAL(new_adc_data(ulong)),this,SLOT(display_data(ulong)));
    thread->start();

    /* Add graph and set the curve line color to green */
    ui->CustomPlot->addGraph();
    ui->CustomPlot->graph(0)->setPen(QPen(Qt::red));
    ui->CustomPlot->graph(0)->setAntialiasedFill(false);

    /* Configure x-Axis as time in secs */
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    ui->CustomPlot->xAxis->setTicker(timeTicker);
    ui->CustomPlot->axisRect()->setupFullAxesBox();


    /* Configure x and y-Axis to display Labels */
    ui->CustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(),8));
    ui->CustomPlot->yAxis->setTickLabelFont(QFont(QFont().family(),8));
    ui->CustomPlot->xAxis->setLabel("Time(s)");
    ui->CustomPlot->yAxis->setLabel("ADC Raw Counts");

    /* Make top and right axis visible, but without ticks and label */
    ui->CustomPlot->xAxis2->setVisible(true);
    ui->CustomPlot->yAxis->setVisible(true);
    ui->CustomPlot->xAxis2->setTicks(false);
    ui->CustomPlot->yAxis2->setTicks(false);
    ui->CustomPlot->xAxis2->setTickLabels(false);
    ui->CustomPlot->yAxis2->setTickLabels(false);

    /* Set up and initialize the graph plotting timer */
    connect(&timer_plot, SIGNAL(timeout()),this,SLOT(realtimePlot()));
    timer_plot.start(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************************************************************
 * Function Name : display_data
 * Description   : Displays ADC data on the GUI
 * Returns       : None
 * Params        @adc_data: Data to be displayed
 ****************************************************************/
void MainWindow::display_data(unsigned long adc_data)
{
    /* Discard the noisy bits */
    adc_data = adc_data/100;

    /* Support the plotter */
    adc_data_g = adc_data;

    /* Prepare to display the data */
    QString adc_data_str = " ";
    adc_data_str = QString::number(adc_data);
    ui->label_adc_data->setText(adc_data_str);
}

/****************************************************************
 * Function Name : realtimePlot
 * Description   : Displays the real time graph on the GUI
 * Returns       : None
 * Params        : None
 ****************************************************************/
void MainWindow::realtimePlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;
    if(key - lastPointKey > 0.002)
    {
        ui->CustomPlot->graph(0)->addData(key, (double)adc_data_g);
        lastPointKey = key;
    }

    /* make key axis range scroll right with the data at a constant range of 8. */
    ui->CustomPlot->graph(0)->rescaleValueAxis();
    ui->CustomPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->CustomPlot->replot();
}

/****************************************************************
 * Function Name : on_pushButton_released
 * Description   : Displays THANK YOU! when pressed and released
 * Returns       : None
 * Params        : None
 ****************************************************************/
void MainWindow::on_pushButton_released()
{
    ui->label_ty->setText("THANK YOU!");
}
