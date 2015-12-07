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
        emit statusUpdate("<span style=\"color: green;\">Connected</span>");
    else
        emit statusUpdate("<span style=\"color: red;\">Couldn't connect: " + spp->errorString() + "</span>");
}

void BluetoothServer::stop()
{
    spp->close();
}

void BluetoothServer::readyRead()
{
    /* Read 3 bytes of data. 1 byte ID and 2 byte Data. */
    QByteArray barr = spp->read(3); // Read available bytes but no more than 3 bytes.
    while( barr.size() < 3 )   // Keep reading until 3 bytes has been read.
    {
        spp->waitForReadyRead(10);  // Chilla until more bytes are available
        barr.append(spp->read(3 - barr.size()));    // Read new avaliable bytes to read ones, no more than 3 total.
    }

    /* Convert read bytes to unsigned integers. */
    quint8 id = barr.at(0);     // First(0) byte is ID.
    quint16 data = barr.at(2);  // Second(1) byte is hdata and third(2) is ldata

    //emit statusUpdate(QString(QString::number(id) + " " + QString::number(data)));
    /* === Convert 2 chars to integer === *\
    /* int result = high + (low >> (CHAR_BIT - 2)); */
    //quint16 data = buf[1] + (buf[2] >> (CHAR_BIT - 2));   // Second(1) byte is hdata and thired(2) is ldata.

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
