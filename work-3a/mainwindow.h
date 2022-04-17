#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class QPlainTextEdit;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_help_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_quickeSave_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_open_read_only_clicked();

//    void onMenuActionClicked();

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
    QPlainTextEdit* help_widget;
    QMenuBar* menuBar;
    QMenu* menuLeng;
    QAction* setRu;
    QAction* setEn;
    QTranslator translator;

};
#endif // MAINWINDOW_H
