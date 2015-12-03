#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <stdlib.h>     /* strtol */

// === FireFly Information ===
// Address: 00:06:66:03:71:B7
// Name:    Firefly-71B7
// Class:   0x001f00
// UUID:    00001101-0000-1000-8000-00805f9b34fb

#define FIREFLY_ADDRESS "00:06:66:03:71:B7" // MAC-ADDRESS for FireFly on Güntech robot.
#define FIREFLY_UUID "00001101-0000-1000-8000-00805f9b34fb"

/*
 *  DISCLAIMER: You need to already have paired with the FireFly module.
 */

class BluetoothServer : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothServer(QObject *parent = 0);
    void start();
    void stop();

signals:
    void statusUpdate(const QString &status);

private:
    int s;
    struct sockaddr_rc addr;

};

#endif // BLUETOOTHSERVER_H
