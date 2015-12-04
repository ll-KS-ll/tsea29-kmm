/*
 *  DISCLAIMER: You need to already have paired with the FireFly module and have a serial port to it.
 */

#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <QSerialPort>

// === FireFly Information ===
// Address: 00:06:66:03:71:B7
// Name:    Firefly-71B7
// Class:   0x001f00
// UUID:    00001101-0000-1000-8000-00805f9b34fb
#define FIREFLY_ADDRESS "00:06:66:03:71:B7" // MAC-ADDRESS for FireFly on Güntech robot.


class BluetoothServer : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothServer(QObject *parent = 0);
    void start(const QString &serialPortName);   // Start Bluetooth server.
    void stop();    // Stop Bluetooth server.

signals:
    void statusUpdate(const QString &status);               // New status. Ex. connected.
    void updateData(const quint8 &id, const quint16 &data); // New data package read.

private slots:
    void readyRead(); // Incomming data.

private:
    QSerialPort *spp;   // Serial Port for connectiong to FireFly.

};

#endif // BLUETOOTHSERVER_H
