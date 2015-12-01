#ifndef BLUETOOTHSERVER_H
#define BLUETOOTHSERVER_H

#include <QObject>
#include <QBluetoothLocalDevice>            // Local Bluetooth Device Controller.
#include <QBluetoothDeviceDiscoveryAgent>   // Bluetooth Discovery Agent.

// === FireFly Information ===
// Address: 00:06:66:03:71:B7
// Name:    Firefly-71B7
// Class:   0x001f00
// UUID:    00001101-0000-1000-8000-00805f9b34fb
#define FIREFLY_ADDRESS QBluetoothAddress("00:06:66:03:71:B7") // MAC-ADDRESS for FireFly on Güntech robot.

class BluetoothServer : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothServer(QObject *parent = 0);
    void startServer();

signals:
    void statusUpdate(const QString &status);
    void error(const QBluetoothDeviceDiscoveryAgent::Error &error);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void discoveryFinished();

private:
    QBluetoothLocalDevice *localDevice;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothDeviceInfo *remoteDevice;
};

#endif // BLUETOOTHSERVER_H
