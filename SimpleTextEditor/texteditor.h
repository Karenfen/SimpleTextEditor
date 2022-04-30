#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTranslator>


QT_BEGIN_NAMESPACE
namespace Ui { class textEditor; }
QT_END_NAMESPACE

class QPlainTextEdit;


class textEditor : public QMainWindow
{
    Q_OBJECT

public:
    textEditor(QWidget *parent = nullptr);
    ~textEditor();

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

    void setLightTheme();
    void setDarkTheme();

private:

    Ui::textEditor *ui;
    QString currentFilePath;
    QPlainTextEdit* help_widget;
    QPlainTextEdit* changeKeyWidjet;
    QMenu* menuLeng;
    QAction* setRu;
    QAction* setEn;
    QMenu* menuKey;
    QAction* setKeySave;
    QAction* setKeyOpen;
    QAction* setKeyQuit;
    QAction* setKeyClose;
    QAction* showKeys;
    QMenu* menuTheme;
    QAction* lightTheme;
    QAction* darkTheme;
    QTranslator* translator;
    std::map<QString, int>hotKeys;

    QString hoKeyList();

public:

    void setText();
    void personalization();

protected:

    bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif // TEXTEDITOR_H
