#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Init Bluetooth. */
    btServer = new BluetoothServer(this);
    connect(btServer, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdated(QString)));
    connect(btServer, SIGNAL(updateData(quint8,quint16)), this, SLOT(updateData(quint8,quint16)));
}

MainWindow::~MainWindow()
{
    btServer->stop();
    delete btServer;
    delete ui;
}

void MainWindow::statusUpdated(const QString &status)
{
    ui->logTextBox->append("[BT] " + status);
}

void MainWindow::updateData(const quint8 &id, const quint16 &data)
{
    /* Data recived, update gui element. */
    switch (id) {
        case IR_FRONT:
            ui->irFront->setText(QString::number(data));
            break;
        case IR_LEFT_FRONT:
            ui->irFrontLeft->setText(QString::number(6050 / data));
            break;
        case IR_RIGTH_FRONT:
            ui->irFrontRight->setText(QString::number(6050 / data));
            break;
        case IR_LEFT_BACK:
            ui->irBackLeft->setText(QString::number(6050 / data));
            break;
        case IR_RIGTH_BACK:
            ui->irBackRight->setText(QString::number(6050 / data));
            break;
        case LINE_SENSOR1:
            ui->reflex1->setText(QString::number(data));
            break;
        case LINE_SENSOR2:
            ui->reflex2->setText(QString::number(data));
            break;
        case LINE_SENSOR3:
            ui->reflex3->setText(QString::number(data));
            break;
        case LINE_SENSOR4:
            ui->reflex4->setText(QString::number(data));
            break;
        case LINE_SENSOR5:
            ui->reflex5->setText(QString::number(data));
            break;
        case LINE_SENSOR6:
            ui->reflex6->setText(QString::number(data));
            break;
        case LINE_SENSOR7:
            ui->reflex7->setText(QString::number(data));
            break;
        default:
            ui->logTextBox->append(QString("<span style=\"color: orange;\">Unrecognized id: [" +
                                           QString::number(id) + ": " +QString::number(data) + "]"));
            break;
    }
}

void MainWindow::startBluetooth()
{
    statusUpdated(QString("Connecting..."));
    btServer->start();
}
