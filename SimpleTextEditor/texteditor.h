#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTranslator>
#include <memory>
#include <QTextCharFormat>

#include "filer.h"


QT_BEGIN_NAMESPACE
namespace Ui { class textEditor; }
QT_END_NAMESPACE

class QPlainTextEdit;
class QTextEdit;


class textEditor : public QMainWindow
{
    Q_OBJECT

public:
    textEditor(QWidget *parent = nullptr);
    ~textEditor();

private slots:
// кнопки
    void on_pushButton_save_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_open_read_only_clicked();
// меню
    void onMenuLangClicked();
    void onMenuKeyCliced();
    void onMenuKeyInfo();
    void on_pushButton_help_clicked();
    void setTheme();
// проводгик
    void filerReturnPath(const QString& path);
// туллбар
    void on_print_clicked();
    void on_pushButton_quickeSave_clicked();
    void on_showFiles_clicked();
    void copy_past_font();
    void setTextAlignment();
    void selectAllText();

private:

    std::unique_ptr<Ui::textEditor> ui;
    std::unique_ptr<QPlainTextEdit> help_widget;
    std::unique_ptr<QPlainTextEdit> changeKeyWidjet;
    std::unique_ptr<filer> fileView;

    QTextEdit* plaintext;

    QMenu* menuLeng;
    QMenu* menuKey;
    QMenu* menuTheme;

    QTranslator* translator;


    std::map<QString, int>hotKeys;
    std::map<QString, QAction*> m_actions;

    QTextCharFormat currentFont;

private:

    QString hoKeyList();
    bool textEditIsValid();

public:

    void setText();
    void personalization();

protected:

    bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif // TEXTEDITOR_H
