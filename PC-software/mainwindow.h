#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bluetoothserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startBluetooth();

public slots:
    void statusUpdated(const QString &status);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    BluetoothServer *btServer;
};

#endif // MAINWINDOW_H
