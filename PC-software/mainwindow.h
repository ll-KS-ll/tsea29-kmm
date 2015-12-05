#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bluetoothserver.h"
#include <math.h>

/* === ID CODES === */
#define BUTTON_START 0
#define BUTTON_AUTO 1

#define IR_LEFT_FRONT 3
#define IR_LEFT_BACK 4
#define IR_FRONT 5
#define IR_RIGTH_BACK 6
#define IR_RIGTH_FRONT 7
#define LINE_SENSOR1 10
#define LINE_SENSOR2 11
#define LINE_SENSOR3 12
#define LINE_SENSOR4 13
#define LINE_SENSOR5 14
#define LINE_SENSOR6 15
#define LINE_SENSOR7 16
#define GYRO 17
/* ================ */

#define VOLTS_PER_UNIT 0.0049

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void statusUpdated(const QString &status);  // New Bluetooth status.
    void updateData(const quint8 &id, const quint16 &data); // New data package recived.

private slots:
    void on_connect_clicked();  // Connect to FireFly

private:
    Ui::MainWindow *ui;
    BluetoothServer *btServer;  // Bluetooth server for connecting to FireFly.
};

#endif // MAINWINDOW_H
