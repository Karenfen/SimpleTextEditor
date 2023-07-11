#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <memory>


class QUdpSocket;
class QTcpServer;
class QTcpSocket;


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readingDatagramsFromUDP();
    void readingDataFromTCP();
    void newConection();
    void closeSocket();
    void startServer();
    void stopServer();

signals:
    void writeMessageToList(const QString& message);

private:
    std::unique_ptr<QUdpSocket> UDPsocket;
    std::unique_ptr<QTcpServer> TCPserver;
    QTcpSocket* currentSocket{};
};


#endif // MAINWINDOW_H
