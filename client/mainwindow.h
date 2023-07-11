#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QAbstractSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QUdpSocket;
class QTcpSocket;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void sendDatagramFromUdp();
    void sendDatagramFromTcp();
    bool hostSettingsChanged();


private slots:
    void sendMessage();
//    void acceptErrorSocket(QAbstractSocket::SocketError socketError);


private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<QUdpSocket> UDPsocket;
    std::unique_ptr<QTcpSocket> TCPsocket;

};

#endif // MAINWINDOW_H
