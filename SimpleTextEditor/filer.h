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

signals:
    void fileSelected(QString);

private slots:
    void on_quit_clicked();
    void selectionChanged();

    void on_open_clicked();

    void on_setPath_clicked();

private:
    Ui_Form* ui;
    std::shared_ptr<QDirModel> m_model;

};

#endif // FILER_H
