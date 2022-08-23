#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QMessageBox>

#define _TCP_ "TCP"
#define _UDP_ "UDP"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// создаем сокеты
    UDPsocket = std::make_unique<QUdpSocket>(this);
    TCPsocket = std::make_unique<QTcpSocket>(this);

// настраиваем графические элементы
    ui->protocolComboBox->addItem(_TCP_);
    ui->protocolComboBox->addItem(_UDP_);

    ui->ipAdderssLineEdit->setInputMask("000.000.000.000;");
    ui->ipAdderssLineEdit->setText("127.0.0.1");

    ui->portSpinBox->setValue(443);

// создаём связи
    connect(ui->sendPushButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(TCPsocket.get(), &QTcpSocket::connected, this, &MainWindow::sendDatagramFromTcp);
//    connect(TCPsocket.get(), SIGNAL(error), this, SLOT(acceptErrorSocket));
    connect(TCPsocket.get(), &QTcpSocket::disconnected, TCPsocket.get(), &QTcpSocket::abort);
}

MainWindow::~MainWindow()
{

}

void MainWindow::sendMessage()
{
    QByteArray datagram = ui->messagePlainTextEdit->toPlainText().toUtf8();

    if(datagram.isEmpty())
    {
        QMessageBox::warning(this, tr("Внимание!"), tr("Сообщение не должно быть пустым."));
        return;
    }

    if(ui->protocolComboBox->currentText() == _UDP_)
        sendDatagramFromUdp();
    else if(ui->protocolComboBox->currentText() == _TCP_)
    {
        if(hostSettingsChanged())
            TCPsocket->abort();
        QAbstractSocket::SocketState TCPsocketState = TCPsocket->state();

        if(TCPsocketState == QAbstractSocket::UnconnectedState)
        {
            TCPsocket->connectToHost(QHostAddress(ui->ipAdderssLineEdit->text()), ui->portSpinBox->value());
            if(!TCPsocket->waitForConnected(1000))
                QMessageBox::warning(this, tr("Ошибка!"), tr("Ошибка соединения...  \nПроверьте настройки или попробуйте позже."));
        }
        else if(TCPsocketState == QAbstractSocket::ConnectedState)
            sendDatagramFromTcp();
        else
            QMessageBox::warning(this, tr("Ошибка!"), tr("Ошибка соединения...  \nПроверьте настройки или попробуйте позже."));
    }
}

//void MainWindow::acceptErrorSocket(QAbstractSocket::SocketError socketError)
//{
//    QMessageBox::warning(this, tr("Ошибка!"), QString(socketError));
//    TCPsocket->abort();
//}



void MainWindow::sendDatagramFromUdp()
{
    UDPsocket->writeDatagram(ui->messagePlainTextEdit->toPlainText().toUtf8(), QHostAddress(ui->ipAdderssLineEdit->text()), ui->portSpinBox->value());
    QMessageBox::information(this, tr("Статус отправки..."), tr("Сообщение отправлено!"));
}

void MainWindow::sendDatagramFromTcp()
{
    TCPsocket->write(ui->messagePlainTextEdit->toPlainText().toUtf8());
    QMessageBox::information(this, tr("Статус отправки..."), tr("Сообщение отправлено!"));
}

bool MainWindow::hostSettingsChanged()
{
    if(TCPsocket->peerPort() == ui->portSpinBox->value() && TCPsocket->peerAddress() == QHostAddress(ui->ipAdderssLineEdit->text()))
        return false;

    return true;
}
