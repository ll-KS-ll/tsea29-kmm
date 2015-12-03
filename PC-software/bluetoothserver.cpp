#include "bluetoothserver.h"


void baswap(bdaddr_t *dst, const bdaddr_t *src)
{
    register unsigned char *d = (unsigned char *) dst;
    register const unsigned char *s = (const unsigned char *) src;
    register int i;

    for (i = 0; i < 6; i++)
           d[i] = s[5-i];
}

int str2ba(const char *str, bdaddr_t *ba)
{
    uint8_t b[6];
    const char *ptr = str;
    int i;

    for (i = 0; i < 6; i++) {
        b[i] = (uint8_t) strtol(ptr, NULL, 16);
        if (i != 5 && !(ptr = strchr(ptr, ':')))
            ptr = ":00:00:00:00:00";
        ptr++;
    }

    baswap(ba, (bdaddr_t *) b);

    return 0;
}

BluetoothServer::BluetoothServer(QObject *parent) : QObject(parent)
{
    // Allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // Set the connection parameters (who to connect to)
    addr = { 0 };
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    const char dest[18] = FIREFLY_ADDRESS;
    str2ba( dest, &addr.rc_bdaddr);
}

void BluetoothServer::start()
{
    int status;

    // Connect to server
    status = ::connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if( status == 0 )
        emit statusUpdate(QString("<span style=\"color: green;\">Connected</span>"));
    else
        emit statusUpdate(QString("<span style=\"color: red;\">Connect status " + QString::number(status) + "</span>"));
}

void BluetoothServer::stop()
{
    close(s);
}

/*
void BluetoothServer::readSocket()
{
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
*/
