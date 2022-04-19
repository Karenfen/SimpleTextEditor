#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMenuBar>
#include <QApplication>
#include <QMessageBox>



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
    this->help_widget->setWindowIcon(QIcon(":/images/icon2.png"));

    QFile fileTXT(":/docs/help.txt");

    if(fileTXT.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&fileTXT);
        this->help_widget->setPlainText(readText.readAll());

        fileTXT.close();
    }

     /* добавляем выбор языка */

    this->menuBar = new QMenuBar(this);
    this->menuBar->setGeometry(0, 0, 15, 25);

    this->menuLeng = new QMenu(menuBar);
    this->menuLeng->resize(100, 20);

    this->setRu = new QAction(this);
    this->setRu->setText("Русский");
    this->setRu->setObjectName("setRu");
    this->setEn = new QAction(this);
    this->setEn->setText("English");
    this->setEn->setObjectName("setEn");

    this->menuLeng->addAction(setRu);
    this->menuLeng->addAction(setEn);

    this->menuBar->addMenu(menuLeng);

    this->translator = new QTranslator(this);
    connect(setRu, &QAction::triggered, this, &MainWindow::onMenuLangClicked);
    connect(setEn, &QAction::triggered, this, &MainWindow::onMenuLangClicked);

    this->centralWidget()->installEventFilter(this);

    /* создаём виджет для замены комбинаций клавиш */
    this->changeKeyWidjet = new QPlainTextEdit;
    this->changeKeyWidjet->resize(500, 100);
    this->changeKeyWidjet->setReadOnly(true);
    this->changeKeyWidjet->setWindowModality(Qt::ApplicationModal);

    /* создаём набор горячих клавишь */
    hotKeys["save"] = Qt::Key::Key_S;
    hotKeys["open"] = Qt::Key::Key_O;
    hotKeys["close"] = Qt::Key::Key_N;
    hotKeys["quit"] = Qt::Key::Key_Q;

    /* добавляем меню для замены клавишь */
    menuKey = new QMenu(menuBar);

    setKeySave = new QAction(this);
    setKeySave->setObjectName("save");
    setKeyOpen = new QAction(this);
    setKeyOpen->setObjectName("open");
    setKeyQuit = new QAction(this);
    setKeyQuit->setObjectName("quit");
    setKeyClose = new QAction(this);
    setKeyClose->setObjectName("close");
    showKeys = new QAction(this);

    menuKey->addActions({showKeys, setKeyClose, setKeyOpen, setKeySave, setKeyQuit});
    menuBar->addMenu(menuKey);

    connect(setKeySave, &QAction::triggered, this, &MainWindow::onMenuKeyCliced);
    connect(setKeyOpen, &QAction::triggered, this, &MainWindow::onMenuKeyCliced);
    connect(setKeyClose, &QAction::triggered, this, &MainWindow::onMenuKeyCliced);
    connect(setKeyQuit, &QAction::triggered, this, &MainWindow::onMenuKeyCliced);
    connect(showKeys, &QAction::triggered, this, &MainWindow::onMenuKeyInfo);

    changeKeyWidjet->installEventFilter(this);

    /* устанавливаем текст подсказок на установленном языке */
    this->setLeng();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete help_widget;
    delete changeKeyWidjet;
//    delete menuBar;
//    delete menuLeng;
//    delete setRu;
//    delete setEn;
//    delete translator;
//    delete menuKey;
//    delete setKeySave;
//    delete setKeyOpen;
//    delete setKeyQuit;
//    delete setKeyKlose;
}


void MainWindow::on_pushButton_help_clicked()
{
    this->help_widget->show();
}


void MainWindow::on_pushButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
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
        QString fileName = QFileDialog::getSaveFileName(this, tr("выберите файл"),
                                                        QDir::homePath(),
                                                        tr("текст (*.txt)"));
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
    ui->filePathInfo->setText(tr("новый файл"));
    ui->plainTextEdit->setReadOnly(false);
}


void MainWindow::on_pushButton_open_read_only_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
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


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    if(event->type() == QEvent::KeyRelease)
    {
        if(obj == changeKeyWidjet)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

             if(keyEvent->modifiers() == Qt::ControlModifier)
             {
                 QString objName = obj->objectName();

                 if(objName == "save")
                 {
                    hotKeys["save"] = keyEvent->key();
                    QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                    changeKeyWidjet->hide();
                    return true;
                 }
                 if(objName == "close")
                 {
                     hotKeys["close"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
                 if(objName == "quit")
                 {
                     hotKeys["quit"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
                 if(objName == "open")
                 {
                     hotKeys["open"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
             }
        }

        if(obj == centralWidget())
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            if(keyEvent->modifiers() == Qt::ControlModifier)
            {

                if(keyEvent->key() == hotKeys.at("save"))
                {
                    ui->pushButton_save->clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("open"))
                {
                    ui->pushButton_open->clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("close"))

                {
                    ui->pushButton_close->clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("quit"))
                {
                    qApp->exit();
                    return true;
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}


void MainWindow::onMenuLangClicked()
{
    QObject* obj = sender();

    if(obj->objectName() == "setRu")
    {
        qApp->removeTranslator(translator);
    }
    else if(obj->objectName() == "setEn")
    {
        this->translator->load(":/QtLanguage_en");
        qApp->installTranslator(translator);
    }

    ui->retranslateUi(this);
    this->setLeng();

}


void MainWindow::onMenuKeyCliced()
{
    QObject* obj = sender();
    QString objName = obj->objectName();

    if(objName == "save")
    {
        changeKeyWidjet->setObjectName("save");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"сохраение\""));
    }
    else if(objName == "close")
    {
        changeKeyWidjet->setObjectName("close");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"закрыть документ\""));
    }
    else if(objName == "quit")
    {
        changeKeyWidjet->setObjectName("quit");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"закрыть программу\""));
    }
    else if(objName == "open")
    {
        changeKeyWidjet->setObjectName("open");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"открыть файл\""));
    }

    changeKeyWidjet->show();
}


void MainWindow::onMenuKeyInfo()
{
    QMessageBox::information(this, tr("горячие клавиши"), hoKeyList());
}


void MainWindow::setLeng()
{
    this->help_widget->setWindowTitle(tr("Справка"));
    ui->pushButton_close->setToolTip(tr("закрыть текущий файл без сохранения изменений"));
    ui->pushButton_help->setToolTip(tr("открыть текст справки по приложению"));
    ui->pushButton_open->setToolTip(tr("открыть файл для редактирования"));
    ui->pushButton_open_read_only->setToolTip(tr("открыть файл только для просмотра"));
    ui->pushButton_quickeSave->setToolTip(tr("сохранить изменения в открытом файле"));
    ui->pushButton_save->setToolTip(tr("выбрать файл для сохранения изменений"));
    this->menuLeng->setTitle(tr("Язык"));
    this->menuLeng->setToolTip(tr("установить язык интерфейса"));
    menuKey->setTitle(tr("корячие клавиши"));
    menuKey->setToolTip(tr("нажмите для замены комбинации клавишь"));
    setKeySave->setText(tr("сохранить как..."));
    setKeyOpen->setText(tr("открыть новый документ"));
    setKeyQuit->setText(tr("закрыть программу"));
    setKeyClose->setText(tr("закрыть документ не сохраняя"));
    changeKeyWidjet->setPlainText(tr("нажмите CTRL + клавишу для замены"));
    showKeys->setText(tr("посмотреть горячие клавиши"));
}

QString MainWindow::hoKeyList()
{
    QString text{tr("текущие настройки клавишь:\n")};

    for (const auto& element : hotKeys)
    {
        text += element.first;
        text += tr("  -  клавиша: ");
        text += element.second;
        text += "\n";
    }

    return text;
}

