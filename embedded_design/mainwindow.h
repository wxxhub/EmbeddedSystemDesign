#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QProgressBar>
#include <QProgressDialog>

#include "../vision/include/vision_model.h"
#include "../port/include/usb_port.h"

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
    void on_chooseImage_clicked();

    void on_SelectDetectorFunction_valueChanged(int arg1);

    void readPortData(char* data);

    void on_openPort_clicked();

    void on_closePort_clicked();

    void on_clearPort_clicked();

    void on_sendData_clicked();

    void on_clearDetectResult_clicked();

    void on_sendDetectorResult_clicked();

    void on_clearText_clicked();

private:
    Ui::MainWindow *ui;

    vision_model::VisionModel * vision_model_;
    usb_port::UsbPort *usb_port_;
    int function_num_;

    QImage toQImage(cv::Mat image);

    void sendResult();
    char *getDirection(int value);

    boost::thread read_data_thread_;
    boost::thread send_data_thread_;
};

#endif // MAINWINDOW_H
