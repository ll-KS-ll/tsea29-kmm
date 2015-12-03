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

    // Add a socket notifer to our socket for incomming data.
    socketNotifier = new QSocketNotifier(s, QSocketNotifier::Read, this->parent());
    connect(socketNotifier, SIGNAL(activated(int)), this, SLOT(readyRead(int)));
}

void BluetoothServer::start()
{
    int status;

    // Connect to server
    status = ::connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if( status == 0 )
        emit statusUpdate(QString("<span style=\"color: green;\">Connected</span>"));
    else {
        close(s);
        emit statusUpdate(QString("<span style=\"color: red;\">Connect status " + QString::number(status) + "</span>"));
    }
}

void BluetoothServer::stop()
{
    close(s);
}

void BluetoothServer::readyRead(int socket)
{
    char buf[3];    // Read 3 bytes. 1 byte id and 2 bytes data.
    read(socket, buf, 3);   // Read from socket.

    quint8 id = buf[0];     // First(0) byte is ID.
    quint16 data = buf[2];  // Second(1) byte is hdata and third(2) is ldata

    /* === Convert 2 chars to integer === *\
    /* int result = high + (low >> (CHAR_BIT - 2)); */
    //quint16 data = buf[1] + (buf[2] >> (CHAR_BIT - 2));   // Second(1) byte is hdata and thired(2) is ldata.

    if( data != 0 ) // Filter invalid data.
        emit updateData(id, data);
}
