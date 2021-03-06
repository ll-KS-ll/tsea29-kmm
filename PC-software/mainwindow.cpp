#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Init Bluetooth. */
    btServer = new BluetoothServer(this);
    connect(btServer, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdated(QString)));
    connect(btServer, SIGNAL(updateData(quint8,quint16)), this, SLOT(updateData(quint8,quint16)));
    connect(btServer, SIGNAL(sentCommand(quint8)), this, SLOT(sentCommand(quint8)));

    ui->btn_forward->setShortcut(QKeySequence(tr("W")));
    ui->btn_stop->setShortcut(QKeySequence(tr("S")));
    ui->btn_reverse->setShortcut(QKeySequence(tr("X")));

    ui->btn_turnLeft->setShortcut(QKeySequence(tr("A")));
    ui->btn_turnRight->setShortcut(QKeySequence(tr("D")));

    ui->btn_rotateLeft->setShortcut(QKeySequence(tr("Q")));
    ui->btn_rotateRight->setShortcut(QKeySequence(tr("E")));

    ui->btn_calibrate->setShortcut(QKeySequence(tr("C")));
    ui->btn_music->setShortcut(QKeySequence(tr("M")));
}

MainWindow::~MainWindow()
{
    btServer->stop();
    delete btServer;
    delete ui;
}

void MainWindow::statusUpdated(const QString &status)
{
    ui->logTextBox->append("[BT] " + status);
}

/* Convert Steer Command integer ID to a human readable string. */
QString cmd2str(const quint16 &data)
{
    switch (data) {
    case CMD_NOP:
        return "No command";
        break;
    case CMD_FORWARD:
        return "Drive forward";
        break;
    case CMD_STOP:
        return "Stop";
        break;
    case CMD_REVERSE:
        return "Drive backwards";
        break;
    case CMD_RIGHT:
        return "Turn right";
        break;
    case CMD_LEFT:
        return "Turn left";
        break;
    case CMD_ROTATE_R:
        return "Rotate right";
        break;
    case CMD_ROTATE_L:
        return "Rotate left";
        break;
    case CMD_CALIBRATE:
        return "Calibrate linesensor";
        break;
    case CMD_FOLLOW_TAPE:
        return "Follow tape";
        break;
    case CMD_MUSIC:
        return "Play music";
        break;
    default:
        return "Unknown command";
    }
}

void MainWindow::updateData(const quint8 &id, const quint16 &data)
{
    /* Data recived, update gui element. */
    switch (id) {
        case IR_FRONT:
            ui->irFront->setText(QString::number(data) + " cm");
            break;
        case IR_LEFT_FRONT:
            ui->irFrontLeft->setText(QString::number(data) + " cm");
            break;
        case IR_RIGTH_FRONT:
            ui->irFrontRight->setText(QString::number(data) + " cm");
            break;
        case IR_LEFT_BACK:
            ui->irBackLeft->setText(QString::number(data) + " cm");
            break;
        case IR_RIGTH_BACK:
            ui->irBackRight->setText(QString::number(data) + " cm");
            break;
        case LINE_SENSOR:
            // Old ID for the linesensor that is sent for no good reason.
            break;
        case LINESENSOR:
            ui->reflex0->setText( "[" + QString::number( (data & 0x01)>>0 ) + "]");
            ui->reflex1->setText( "[" + QString::number( (data & 0x02)>>1 ) + "]");
            ui->reflex2->setText( "[" + QString::number( (data & 0x04)>>2 ) + "]");
            ui->reflex3->setText( "[" + QString::number( (data & 0x08)>>3 ) + "]");
            ui->reflex4->setText( "[" + QString::number( (data & 0x10)>>4 ) + "]");
            ui->reflex5->setText( "[" + QString::number( (data & 0x20)>>5 ) + "]");
            ui->reflex6->setText( "[" + QString::number( (data & 0x40)>>6 ) + "]");
            break;
        case STY_CMD:
            if (ui->lbl_activeCmd->text() != cmd2str(data) && data != 0) {
                ui->lbl_activeCmd->setText(cmd2str(data));
                ui->logTextBox->append(cmd2str(data));
            }
            break;
        case REMOTE_CMD:
            if (data == CMD_CALIBRATE)
                ui->logTextBox->append("Pressed autonom button");
            break;
        case BUTTON_AUTO:
            if(data == 1) {
                ui->logTextBox->append("Pressed autonom button");
                if (ui->lbl_auto->text() == MODE_REMOTE)
                    ui->lbl_auto->setText(MODE_AUTO);
                else
                    ui->lbl_auto->setText(MODE_REMOTE);
            }
            break;
        case BUTTON_START:
            if(data == 1) {
                ui->logTextBox->append("Pressed start button");
                if (ui->lbl_start->text() == MODE_WAIT)
                    ui->lbl_start->setText(MODE_STARTED);
                else
                    ui->lbl_start->setText(MODE_WAIT);
            }
            break;
        case TAPE_FOUND:
            if(data == 1)
                ui->logTextBox->append("Tape found");
            break;

        default:
            ui->logTextBox->append(QString("<span style=\"color: orange;\">Unrecognized id: [" +
                                           QString::number(id) + ": " +QString::number(data) + "]</span>"));
            break;
    }
}

void MainWindow::sentCommand(const quint8 &cmd)
{
    ui->logTextBox->append("Sent command: " + cmd2str(cmd));
}

void MainWindow::on_connect_clicked()
{
    btServer->start(ui->serialPortName->text());
    this->focusNextChild();
}

void MainWindow::on_btn_forward_clicked() { btServer->writeCommand(CMD_FORWARD); }

void MainWindow::on_btn_stop_clicked() { btServer->writeCommand(CMD_STOP); }

void MainWindow::on_btn_reverse_clicked() { btServer->writeCommand(CMD_REVERSE); }

void MainWindow::on_btn_turnRight_clicked() { btServer->writeCommand(CMD_RIGHT); }

void MainWindow::on_btn_turnLeft_clicked() { btServer->writeCommand(CMD_LEFT); }

void MainWindow::on_btn_rotateRight_clicked() { btServer->writeCommand(CMD_ROTATE_R); }

void MainWindow::on_btn_rotateLeft_clicked() { btServer->writeCommand(CMD_ROTATE_L); }

void MainWindow::on_btn_calibrate_clicked() { btServer->writeCommand(CMD_CALIBRATE); }

void MainWindow::on_btn_music_clicked() { btServer->writeCommand(CMD_MUSIC);}
