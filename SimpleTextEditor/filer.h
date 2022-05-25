#ifndef FILER_H
#define FILER_H

#include <QWidget>
#include <QListWidget>
#include <QFileSystemModel>
#include "ui_filer.h"
#include "finder.h"
#include <memory>


class filer : public QWidget
{
    Q_OBJECT

public:
    filer(QWidget *parent = nullptr, const QString& filter = "");
    ~filer();

    void retranslateUi();

signals:
    void fileSelected(QString);

private slots:

    void selectionChanged();

    void on_open_clicked();
    void on_copyPath_clicked();
    void on_search_clicked();

    void addResult(const QString& fileName, const QString& filePath);
    void selectFile(QListWidgetItem* item);

private:
    std::unique_ptr<Ui_Form> ui;
    std::unique_ptr<QFileSystemModel> m_model;
    QListWidget* list;
    Controller* threadControl;

};

#endif // FILER_H
