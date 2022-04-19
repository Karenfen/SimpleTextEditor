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

    void onMenuLangClicked();
    void onMenuKeyCliced();
    void onMenuKeyInfo();

private:

    Ui::MainWindow *ui;
    QString currentFilePath;
    QPlainTextEdit* help_widget;
    QPlainTextEdit* changeKeyWidjet;
    QMenuBar* menuBar;
    QMenu* menuLeng;
    QAction* setRu;
    QAction* setEn;
    QMenu* menuKey;
    QAction* setKeySave;
    QAction* setKeyOpen;
    QAction* setKeyQuit;
    QAction* setKeyClose;
    QAction* showKeys;
    QTranslator* translator;
    std::map<QString, int>hotKeys;

    void setLeng();
    QString hoKeyList();

protected:

    bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif // MAINWINDOW_H
