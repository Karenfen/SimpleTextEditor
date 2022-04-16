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
    this->setWindowTitle("Simple Text Editor");

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

     /* добавляем подсказки для кнопок */

    ui->pushButton_close->setToolTip("закрыть текущий файл без сохранения изменений");
    ui->pushButton_help->setToolTip("открыть текст справки по приложению");
    ui->pushButton_open->setToolTip("открыть файл для редактирования");
    ui->pushButton_open_read_only->setToolTip("открыть файл только для просмотра");
    ui->pushButton_quickeSave->setToolTip("сохранить изменения в открытом файле");
    ui->pushButton_save->setToolTip("выбрать файл для сохранения изменений");
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
        ui->filePathInfo->setText(fileName);
        ui->plainTextEdit->setReadOnly(false);
    }
}


void MainWindow::on_pushButton_save_clicked()
{
    if(!ui->plainTextEdit->isReadOnly())
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
            ui->filePathInfo->setText(fileName);
        }
    }
}


void MainWindow::on_pushButton_quickeSave_clicked()
{
    if(!ui->plainTextEdit->isReadOnly())
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
}


void MainWindow::on_pushButton_close_clicked()
{
    ui->plainTextEdit->clear();
    this->currentFilePath = "";
    ui->filePathInfo->setText("новый файл");
    ui->plainTextEdit->setReadOnly(false);
}


void MainWindow::on_pushButton_open_read_only_clicked()
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
        ui->filePathInfo->setText(fileName);

        ui->plainTextEdit->setReadOnly(true);
    }
}

