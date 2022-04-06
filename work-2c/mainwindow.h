#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QStandardItemModel;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void start();

private slots:
    void on_painting_clicked();

    void on_defaultColor_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* myModel;
};
#endif // MAINWINDOW_H
