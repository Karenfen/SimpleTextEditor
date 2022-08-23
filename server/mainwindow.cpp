#include "mainwindow.h"
#include <QtNetwork>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QListView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>


#define _PORT_ 443
#define _TCP_ "TCP"
#define _UDP_ "UDP"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , currentSocket(nullptr)
{
// настраиваем виджет
    auto Vlayout = new QVBoxLayout{};
    auto Hlayout = new QHBoxLayout{};
    auto startButton = new QPushButton(tr("Старт"));
    auto exitButton = new QPushButton(tr("Выход"));
    auto stopButton = new QPushButton(tr("Стоп"));
    auto messageList = new QListView{};
    auto model = new QStandardItemModel{};
    messageList->setModel(model);
    messageList->setMinimumSize(240, 240);

    Vlayout->addWidget(messageList);
    Hlayout->addWidget(startButton);
    Hlayout->addWidget(stopButton);
    Hlayout->addWidget(exitButton);
    Vlayout->addLayout(Hlayout);

    setLayout(Vlayout);


// инициализируем сокеты
    UDPsocket = std::make_unique<QUdpSocket>(this);
    TCPserver = std::make_unique<QTcpServer>(this);


// связи
    connect(UDPsocket.get(), &QUdpSocket::readyRead, this, &MainWindow::readingDatagramsFromUDP);
    connect(TCPserver.get(), &QTcpServer::newConnection, this, &MainWindow::newConection);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopServer);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(this, &MainWindow::writeMessageToList, model, [model](const QString& message)
    {
        model->appendRow(new QStandardItem(message));
    });
}

MainWindow::~MainWindow()
{
}

inline void MainWindow::readingDatagramsFromUDP()
{
    QByteArray datagram;

    while (UDPsocket->hasPendingDatagrams()) {
        datagram.resize(int(UDPsocket->pendingDatagramSize()));
        UDPsocket->readDatagram(datagram.data(), datagram.size());
        QMessageBox::information(this, tr("Полученно сообщение!"), QString(datagram));
        writeMessageToList(QString(datagram));
    }
}

void MainWindow::readingDataFromTCP()
{
    QByteArray datagram = currentSocket->readAll();
    QMessageBox::information(this, tr("Полученно сообщение!"), QString(datagram));
    writeMessageToList(QString(datagram));
}

void MainWindow::newConection()
{
    currentSocket = TCPserver->nextPendingConnection();
    if(!currentSocket)
    {
        QMessageBox::warning(this, tr("Ошибка соединения!"), tr("Не удалось соединиться с удалённым пользователем..."));
        return;
    }
    connect(currentSocket, &QTcpSocket::readyRead, this, &MainWindow::readingDataFromTCP);
    connect(currentSocket, &QTcpSocket::disconnected, this, &MainWindow::closeSocket);
}

void MainWindow::closeSocket()
{
    if(currentSocket)
    {
        currentSocket->abort();
        currentSocket = nullptr;
    }
}

void MainWindow::startServer()
{
    if(!UDPsocket->isOpen())
    {
        if(!UDPsocket->open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Ошибка сокета!"), tr("Ошибка открытия UDP сокета! Приложение будет закрыто."));
            qApp->exit();
        }
        if(!UDPsocket->bind(QHostAddress::LocalHost, _PORT_))
        {
            QMessageBox::critical(this, tr("Ошибка сети!"), tr("Ошибка соединения по протоколу UDP! Приложение будет закрыто."));
            qApp->exit();
        }
    }
    if(!TCPserver->isListening())
    {
        if(!TCPserver->listen(QHostAddress::LocalHost, _PORT_))
        {
            QMessageBox::critical(this, tr("Ошибка сервера!"), tr("Ошибка открытия TCP сервера! Приложение будет закрыто."));
            qApp->exit();
        }
    }

    setWindowTitle(tr("Сервер работает..."));
}

void MainWindow::stopServer()
{
    UDPsocket->close();
    TCPserver->close();
    closeSocket();
    setWindowTitle(tr("Сервер выключен!"));
}
