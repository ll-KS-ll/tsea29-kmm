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

public slots:
    void statusUpdated(const QString &status);
    void error(const QBluetoothDeviceDiscoveryAgent::Error &error);

private:
    Ui::MainWindow *ui;
    BluetoothServer *btServer;
};

#endif // MAINWINDOW_H
