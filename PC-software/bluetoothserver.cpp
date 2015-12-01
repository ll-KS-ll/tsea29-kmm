#include "bluetoothserver.h"


BluetoothServer::BluetoothServer(QObject *parent) : QObject(parent)
{
    QBluetoothLocalDevice *localDevice = new QBluetoothLocalDevice();

    emit statusUpdate(QString("Connecting..."));   // Doesn't work but but...

    if (localDevice->isValid()) {
        /* Init local Bluetooth device. */
        localDevice->powerOn(); // Turn Bluetooth on
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);   // Make it connectable to others

        /* Discovery */
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
        connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
        connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)));
        connect(discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
        // Start a discovery
        emit statusUpdate(QString("Start bt scan."));   // Doesn't work but but...
        discoveryAgent->start();
    }
}

void BluetoothServer::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if(device.address() == FIREFLY_ADDRESS) {
        emit statusUpdate(QString("Güntech robot discovered."));
        remoteDevice = new QBluetoothDeviceInfo(device);
        // discoveryAgent->stop(); Causes program to crash unexpectedly :s
    }
}

void BluetoothServer::discoveryFinished()
{
    QList<QBluetoothDeviceInfo> deviceList = discoveryAgent->discoveredDevices();
    for( int i = 0; i < deviceList.size(); i++) {
       QBluetoothDeviceInfo device = deviceList.at(i);
       if(device.address() == FIREFLY_ADDRESS) {
            remoteDevice = new QBluetoothDeviceInfo(device);
            emit statusUpdate(QString("Güntech robot discovered."));
            return;
       }
    }
    emit statusUpdate(QString("Couldn't find Güntech robot :'("));
}
