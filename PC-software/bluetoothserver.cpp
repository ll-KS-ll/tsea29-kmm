#include "bluetoothserver.h"


BluetoothServer::BluetoothServer(QObject *parent) : QObject(parent)
{
    spp = new QSerialPort(this->parent());
    connect(spp, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void BluetoothServer::start(const QString &serialPortName)
{
    spp->close();
    spp->setPortName(serialPortName);
    if (spp->open(QIODevice::ReadWrite))
        emit statusUpdate("<span style=\"color: green;\">Opened port " + serialPortName + "</span>");
    else
        emit statusUpdate("<span style=\"color: red;\">Couldn't connect: " + spp->errorString() + "</span>");
}

void BluetoothServer::stop()
{
    spp->close();
}

void BluetoothServer::readyRead()
{
    /* Read 2 bytes of data. 1 byte ID and 1 byte Data. */
    QByteArray barr = spp->read(2); // Read available bytes but no more than 2 bytes.
    while( barr.size() < 2 )   // Keep reading until 2 bytes has been read.
    {
        spp->waitForReadyRead(1);  // Chill 1ms until more bytes are available
        barr.append(spp->read(2 - barr.size()));    // Read new avaliable bytes to read ones, no more than 2 total.
    }

    /* Convert read bytes to unsigned integers. */
    quint8 id = barr.at(0);     // First(0) byte is ID.
    quint16 data = barr.at(1);  // Second(1) byte is Data

    emit updateData(id, data);
}

void BluetoothServer::writeCommand(const quint8 &sty_cmd)
{
    char data = sty_cmd;
    int written = spp->write(&data);
    spp->flush();

    if (written = 1)
        emit sentCommand(sty_cmd);
    else
        emit statusUpdate("<span style=\"color: red;\">Couldn't write command.</span>");
}
