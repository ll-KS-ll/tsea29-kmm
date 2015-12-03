/*
 *  DISCLAIMER: You need to already have paired with the FireFly module.
 */

#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <QSocketNotifier>

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
    void start();   // Start Bluetooth server.
    void stop();    // Stop Bluetooth server.

signals:
    void statusUpdate(const QString &status);   // New status. Ex. connected.
    void updateData(const quint8 &id, const quint16 &data); // New data package read.

private slots:
    void readyRead(int socket); // Incomming data.

private:
    int s;  // Socket for connection.
    struct sockaddr_rc addr;         // Socket address data of FireFly.
    QSocketNotifier *socketNotifier; // Socket notifier to check for incomming data.
};

#endif // BLUETOOTHSERVER_H
