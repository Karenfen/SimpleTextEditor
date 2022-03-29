#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_task1_pushButton_solve_clicked();

    void on_task1_pushButton_clear_clicked();

    void on_task2_pushButton_clear_clicked();

    void on_task2_pushButton_solve_clicked();

    void on_task3_pushButton_ensertHTML_clicked();

    void on_task3_pushButton_clearT2_clicked();

    void on_task3_pushButton_replace_clicked();

    void on_task3_pushButton_add_clicked();

    void on_task3_pushButton_clearT1_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
