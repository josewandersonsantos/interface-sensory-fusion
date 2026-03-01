#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    serialPort.Stop();
    delete ui;
}

void MainWindow::on_btnOpenSerialPort_released()
{
    if( ui->cmbSerialPorts->currentIndex() < 0)
    {
        return;
    }

    if(serialPort.IsOpen())
    {
        serialPort.Stop();
        ui->btnOpenSerialPort->setText("Open");
        ui->btnRefreshSerialPort->setEnabled(true);
        ui->cmbSerialPorts->setEnabled(true);
        return;
    }

    QString port = ui->cmbSerialPorts->itemText(ui->cmbSerialPorts->currentIndex());
    if(serialPort.Start(port.toStdString(), 9600))
    {
        serialPort.SetOnRx([this](char* data, size_t len)
        {
           QString line = QString::fromUtf8(data, len);
           QMetaObject::invokeMethod(this, [this, line]()
                                     {
                                         ui->txtBoxOutSerialPort->insertPlainText(line);
                                     }, Qt::QueuedConnection);
        });

        ui->btnOpenSerialPort->setText("Close");
        ui->btnRefreshSerialPort->setEnabled(false);
        ui->cmbSerialPorts->setEnabled(false);
    }
}

void MainWindow::on_btnRefreshSerialPort_released()
{

}

void MainWindow::on_btnRefreshSerialPort_clicked()
{
    std::vector<std::string> ports = serialPort.GetSerialPorts();

    if(ports.size() == 0) return;

    ui->cmbSerialPorts->clear();
    for(int i = 0; i< ports.size(); i++)
    {
        ui->cmbSerialPorts->addItem(ports[i].c_str());
    }
}

