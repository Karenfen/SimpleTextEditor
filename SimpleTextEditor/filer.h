#ifndef FILER_H
#define FILER_H

#include <QWidget>
#include <QDirModel>
#include "ui_filer.h"
#include <memory>


class filer : public QWidget
{
    Q_OBJECT

public:
    filer(QWidget *parent = nullptr, const QString& filter = "");
    ~filer();

    void refresh();
    void retranslateUi();

signals:
    void fileSelected(QString);

private slots:

    void selectionChanged();

    void on_open_clicked();
    void on_copyPath_clicked();

private:
    std::unique_ptr<Ui_Form> ui;
    std::unique_ptr<QDirModel> m_model;

};

#endif // FILER_H
