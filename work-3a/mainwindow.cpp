#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->currentFilePath = "";
    this->setCentralWidget(ui->centralwidget_2); // не понял как сделать это через форму, поэтому сделал так)
    ui->centralwidget->close();
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->setWindowTitle("Новый файл");

    /* создаё фон */

    QPixmap pix_bg(":/images/bg.jpg");
    QPalette myPalette;

    myPalette.setBrush(QPalette::Background, pix_bg);

    this->setPalette(myPalette);

    /* создаём виджет справки */

    this->help_widget = new QPlainTextEdit;
    this->help_widget->setWindowTitle("Справка");
    this->help_widget->setWindowIcon(QIcon(":/images/icon2.png"));

    QFile fileTXT(":/docs/help.txt");

    if(fileTXT.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&fileTXT);
        this->help_widget->setPlainText(readText.readAll());

        fileTXT.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete help_widget;
}


void MainWindow::on_pushButton_help_clicked()
{
    this->help_widget->show();
}


void MainWindow::on_pushButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "выберите файл",
                                                    QDir::homePath(),
                                                    "текст (*.txt)");
    QFile openFile(fileName);
    if(openFile.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&openFile);

        ui->plainTextEdit->setPlainText(readText.readAll());

        openFile.close();

        this->currentFilePath = fileName;
        this->setWindowTitle(fileName);
    }
}


void MainWindow::on_pushButton_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "выберите файл",
                                                    QDir::homePath(),
                                                    "текст (*.txt)");
    QFile openFile(fileName);
    if(openFile.open(QIODevice::WriteOnly))
    {
        QTextStream writeText(&openFile);
        QString text = ui->plainTextEdit->toPlainText();

        openFile.write(text.toLocal8Bit(), text.length());

        openFile.close();

        currentFilePath = fileName;
        this->setWindowTitle(fileName);
    }
}


void MainWindow::on_pushButton_quickeSave_clicked()
{
    if(!currentFilePath.isEmpty())
    {
        QFile openFile(currentFilePath);
        if(openFile.open(QIODevice::WriteOnly))
        {
            QTextStream writeText(&openFile);
            QString text = ui->plainTextEdit->toPlainText();

            openFile.write(text.toLocal8Bit(), text.length());

            openFile.close();
        }
    }
}


void MainWindow::on_pushButton_close_clicked()
{
    ui->plainTextEdit->clear();
    this->currentFilePath = "";
    this->setWindowTitle("Новый файл");
}
