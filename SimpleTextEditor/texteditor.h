#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTranslator>
#include <memory>
#include "filer.h"


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
    void on_pushButton_save_clicked();
    void on_pushButton_quickeSave_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_open_read_only_clicked();

    void onMenuLangClicked();
    void onMenuKeyCliced();
    void onMenuKeyInfo();

    void setTheme();

    void filerReturnPath(const QString& path);

    void on_print_clicked();

    void on_showFiles_clicked();

private:

    Ui::textEditor *ui;
    QPlainTextEdit* help_widget;
    QPlainTextEdit* changeKeyWidjet;
    QPlainTextEdit* plaintext;
    QMenu* menuLeng;
    QMenu* menuKey;
    QMenu* menuTheme;
    QTranslator* translator;
    std::map<QString, int>hotKeys;
    std::shared_ptr<filer> fileView;
    std::map<QString, QAction*> m_actions;

    QString hoKeyList();

public:

    void setText();
    void personalization();

protected:

    bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif // TEXTEDITOR_H
