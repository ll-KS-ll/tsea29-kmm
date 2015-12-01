#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Init Bluetooth. */
    btServer = new BluetoothServer();
    connect(btServer, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdated(QString)));
    connect(btServer, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(error(QBluetoothDeviceDiscoveryAgent::Error)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::error(const QBluetoothDeviceDiscoveryAgent::Error &error)
{
    ui->logTextBox->append("Discovery error: " + error);
}

void MainWindow::statusUpdated(const QString &status)
{
    ui->logTextBox->append("Status: " + status);
}
