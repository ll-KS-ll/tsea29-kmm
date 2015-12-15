#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bluetoothserver.h"

/* === ID CODES === */
#define BUTTON_START 0
#define BUTTON_AUTO 1

#define LINE_SENSOR 2 // Skit blajs.

#define IR_LEFT_FRONT 3
#define IR_LEFT_BACK 4
#define IR_FRONT 5
#define IR_RIGTH_BACK 6
#define IR_RIGTH_FRONT 7

#define STY_CMD 8
#define REMOTE_CMD 9

#define LINESENSOR 10
#define TAPE_FOUND 18
/* ================ */

/* ==== STY CMD ==== */
#define CMD_NOP 0
#define CMD_FORWARD 1
#define CMD_STOP 2
#define CMD_REVERSE 3
#define CMD_RIGHT 4
#define CMD_LEFT 5
#define CMD_ROTATE_R 6
#define CMD_ROTATE_L 7
#define CMD_CALIBRATE 8
#define CMD_FOLLOW_TAPE 9
#define CMD_MUSIC 10
/* ================= */

/* ===== MODES ===== */
#define MODE_REMOTE "Remote"
#define MODE_AUTO "Auto"
#define MODE_WAIT "Wait"
#define MODE_STARTED "Started"
/* ================= */

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
    void sentCommand(const quint8 &cmd);

private slots:
    void on_connect_clicked();  // Connect to FireFly
    void on_btn_forward_clicked();
    void on_btn_stop_clicked();
    void on_btn_reverse_clicked();
    void on_btn_turnRight_clicked();
    void on_btn_turnLeft_clicked();
    void on_btn_rotateRight_clicked();
    void on_btn_rotateLeft_clicked();
    void on_btn_calibrate_clicked();
    void on_btn_music_clicked();

private:
    Ui::MainWindow *ui;
    BluetoothServer *btServer;  // Bluetooth server for connecting to FireFly.
};

#endif // MAINWINDOW_H
