#include <functional>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace std::placeholders;
using namespace cv;
using namespace usb_port;
using namespace vision_model;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->SelectDetectorFunction->setPrefix("识别方案");
    ui->SelectDetectorFunction->setSingleStep(1);
    ui->SelectDetectorFunction->setRange(1, 4);

    ui->SelectPort->setPrefix("/dev/ttyUSB");
    ui->SelectPort->setSingleStep(1);
    ui->SelectPort->setRange(0, 5);

    vision_model_ = new VisionModel();

    usb_port_ = new UsbPort;
    usb_port_->setPortName("/dev/ttyUSB0");
    usb_port_->setBaudRate(115200);
    CallbackFun fun = bind(&MainWindow::readPortData, this, placeholders::_1);
    read_data_thread_ = usb_port_->createReadCallback(fun, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete usb_port_;
    delete vision_model_;
}

void MainWindow::on_chooseImage_clicked()
{
    String link_path = QFileDialog::getOpenFileName(this, tr("open link file"), " ",  tr("imagefile(*.jpg)")).toStdString();
    Mat image = vision_model_->loadImage(link_path);
    vision_model_->resizeTo480(image, image);
    if (!image.empty())
    {
        QImage q_img = toQImage(image);
        QLabel *image_label = new QLabel;
        image_label->setPixmap(QPixmap::fromImage(q_img));
        image_label->resize(QSize(image.cols, image.rows));
        ui->imageScrollArea->setWidget(image_label);

        vision_model_->process(image, function_num_);
        Mat result_image = vision_model_->getResultImage();

        QImage q_img_2= toQImage(result_image);
        QLabel *image_label_2 = new QLabel;
        image_label_2->setPixmap(QPixmap::fromImage(q_img_2));
        image_label_2->resize(QSize(result_image.cols, result_image.rows));
        ui->resultImagescrollArea->setWidget(image_label_2);

        ui->resultTextBrowser->clear();

        vector<vector<int>> result_data = vision_model_->getResultInfo();
        for (int i = 0; i < result_data.size(); i++)
        {
            char info[64] = {'\0'};
            for (int j = 0; j < result_data[i].size(); j++)
            {
                sprintf(info, "%s %d", info, result_data[i][j]);
            }

            sprintf(info, "%s\n", info);
            ui->resultTextBrowser->insertPlainText(QString(info));
        }

        fflush(stdout);
    }
}

QImage MainWindow::toQImage(Mat image)
{
    switch (image.type()) {
    case CV_8UC1:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols, QImage::Format_Grayscale8);
            return img;
        }
        break;

    case CV_8UC3:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 3, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        break;

    case CV_8UC4:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 4, QImage::Format_ARGB32);
            return img;
        }
        break;

    default:
        {
            QImage img;
            return img;
        }
        break;
    }
}

void MainWindow::sendResult()
{
    vector<vector<int>> result_data = vision_model_->getResultInfo();
    int sum = result_data.size() * result_data[0].size();
    int num_count = 0;
    QProgressDialog *progress_dialog = new QProgressDialog;
    progress_dialog->setWindowTitle("发送数据中");
    progress_dialog->setValue(0);
    progress_dialog->show();
    for (int i = 0; i < result_data.size(); i++)
    {
        for (int j = 0; j < result_data[i].size() - 1; j += 2)
        {
            num_count += 2;
            char *data = getDirection(result_data[i][j] * 10 + result_data[i][j+1]);
            usb_port_->writeData(data);
            progress_dialog->setLabelText(QString(data));
            progress_dialog->setValue(100.0*num_count/sum);
            progress_dialog->update();
            sleep(3);
        }
    }

    progress_dialog->setValue(100);
    progress_dialog->hide();
    delete progress_dialog;
}

char *MainWindow::getDirection(int value)
{
    switch (value) {
    case 0:
        return "s";
    case 1:
        return "d";
    case 10:
        return "a";
    case 11:
        return "w";
    default:
        return "q";
    }
}

void MainWindow::on_SelectDetectorFunction_valueChanged(int arg1)
{
    function_num_ = arg1;
}

void MainWindow::readPortData(char *data)
{
    ui->portDataBrowser->insertPlainText(QString(data));
}

void MainWindow::on_openPort_clicked()
{
    usb_port_->openPort();
}

void MainWindow::on_closePort_clicked()
{
    usb_port_->closePort();
}

void MainWindow::on_clearPort_clicked()
{
    usb_port_->clearPort();
}

void MainWindow::on_sendData_clicked()
{
    char* data = ui->sendText->displayText().toLatin1().data();
//    printf("test: %s\n", data);
    usb_port_->writeData(data);
}

void MainWindow::on_clearDetectResult_clicked()
{
    ui->resultTextBrowser->clear();
}

void MainWindow::on_sendDetectorResult_clicked()
{
    send_data_thread_ = boost::thread(boost::bind(&MainWindow::sendResult, this));
}

void MainWindow::on_clearText_clicked()
{
    ui->portDataBrowser->clear();
}
