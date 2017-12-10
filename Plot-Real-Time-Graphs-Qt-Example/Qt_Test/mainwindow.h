/*
* mainwindow.h
*
*  Created on  : Dec 10, 2017
*  Author      : Vinay Divakar
*  website     : www.deeplyemebedded.org
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Includes */
#include <QMainWindow>
#include<QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_released();

public slots:
    void display_data(unsigned long adc_data);
    void realtimePlot();


private:
    Ui::MainWindow *ui;
    unsigned long adc_data_g;
    QTimer timer_plot;
};

#endif // MAINWINDOW_H
