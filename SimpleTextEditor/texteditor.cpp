#include "texteditor.h"
#include "ui_texteditor.h"

#include <QTextStream>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>



textEditor::textEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::textEditor)
{
    ui->setupUi(this);

    translator = new QTranslator;
    help_widget = new QPlainTextEdit;
    changeKeyWidjet = new QPlainTextEdit;
    fileView = std::make_shared<filer>(nullptr, ".txt");


// создаём меню
    /* выбор языка */
    menuLeng = new QMenu(ui->menubar);

    setRu = new QAction(this);
    setEn = new QAction(this);

    setEn->setObjectName("setEn");
    setRu->setObjectName("setRu");

    menuLeng->addActions({setRu, setEn});

    /* горячие клавиши */
    menuKey = new QMenu(ui->menubar);

    setKeySave = new QAction(this);
    setKeyOpen = new QAction(this);
    setKeyQuit = new QAction(this);
    setKeyClose = new QAction(this);
    showKeys = new QAction(this);

    setKeySave->setObjectName("save");
    setKeyOpen->setObjectName("open");
    setKeyQuit->setObjectName("quit");
    setKeyClose->setObjectName("close");

    menuKey->addActions({showKeys, setKeyClose, setKeyOpen, setKeySave, setKeyQuit});

    /* выбор темы */
    menuTheme = new QMenu(ui->menubar);

    lightTheme = new QAction(this);
    darkTheme = new QAction(this);

    menuTheme->addActions({lightTheme, darkTheme});

    ui->menubar->addMenu(menuLeng);
    ui->menubar->addMenu(menuKey);
    ui->menubar->addMenu(menuTheme);


// устанавливаем коннекты
    connect(setKeySave, &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(setKeyOpen, &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(setKeyClose, &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(setKeyQuit, &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(showKeys, &QAction::triggered, this, &textEditor::onMenuKeyInfo);
    connect(setRu, &QAction::triggered, this, &textEditor::onMenuLangClicked);
    connect(setEn, &QAction::triggered, this, &textEditor::onMenuLangClicked);
    connect(lightTheme, &QAction::triggered, this, &textEditor::setLightTheme);
    connect(darkTheme, &QAction::triggered, this, &textEditor::setDarkTheme);
    connect(fileView.get(), SIGNAL(fileSelected(QString)), this, SLOT(filerReturnPath(QString)));

// устанавливаем ивент-фильтры
    centralWidget()->installEventFilter(this);
    changeKeyWidjet->installEventFilter(this);

}

textEditor::~textEditor()
{
    delete ui;
    delete help_widget;
    delete changeKeyWidjet;
}

void textEditor::on_pushButton_help_clicked()
{
    help_widget->show();
}


void textEditor::on_pushButton_open_clicked()
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

        currentFilePath = fileName;
        setStatusTip(fileName);
        ui->plainTextEdit->setReadOnly(false);
    }
}


void textEditor::on_pushButton_save_clicked()
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
            setStatusTip(fileName);
        }
    }
}


void textEditor::on_pushButton_quickeSave_clicked()
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


void textEditor::on_pushButton_close_clicked()
{
    ui->plainTextEdit->clear();
    currentFilePath = "";
    setStatusTip(tr("новый файл"));
    ui->plainTextEdit->setReadOnly(false);
}


void textEditor::on_pushButton_open_read_only_clicked()
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

        currentFilePath = fileName;
        setStatusTip(fileName);

        ui->plainTextEdit->setReadOnly(true);
    }
}


bool textEditor::eventFilter(QObject *obj, QEvent *event)
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
                    textEditor::on_pushButton_save_clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("open"))
                {
                    on_pushButton_open_clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("close"))

                {
                    on_pushButton_close_clicked();
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


void textEditor::onMenuLangClicked()
{
    QObject* obj = sender();

    if(obj->objectName() == "setRu")
    {
        qApp->removeTranslator(translator);
    }
    else if(obj->objectName() == "setEn")
    {
        translator->load(":/QtLanguage_en");
        qApp->installTranslator(translator);
    }

    ui->retranslateUi(this);
    setText();

}


void textEditor::onMenuKeyCliced()
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


void textEditor::onMenuKeyInfo()
{
    QMessageBox::information(this, tr("горячие клавиши"), hoKeyList());
}


void textEditor::setLightTheme()
{
    QFile temeFile(":/themes/light_teme.css");

    if(temeFile.open(QIODevice::ReadOnly))
    {
        QString temeSettings{temeFile.readAll()};

        qApp->setStyleSheet(temeSettings);

        temeFile.close();
    }


    setWindowIcon(QIcon(":/images/icon_dark_cat.png"));

    QPixmap pix_bg(":/images/light_background.jpg");
    QPalette myPalette;

    myPalette.setBrush(QPalette::Background, pix_bg);

    setPalette(myPalette);
}

void textEditor::setDarkTheme()
{
    QFile temeFile(":/themes/dark_teme.css");

    if(temeFile.open(QIODevice::ReadOnly))
    {
        QString temeSettings{temeFile.readAll()};

        qApp->setStyleSheet(temeSettings);

        temeFile.close();
    }

    setWindowIcon(QIcon(":/images/icon_light_cat.png"));

    QPixmap pix_bg(":/images/dark_background.jpg");
    QPalette myPalette;

    myPalette.setBrush(QPalette::Background, pix_bg);

    setPalette(myPalette);
}


void textEditor::setText()
{
    help_widget->setWindowTitle(tr("Справка"));
    ui->pushButton_close->setToolTip(tr("закрыть текущий файл без сохранения изменений"));
    ui->pushButton_help->setToolTip(tr("открыть текст справки по приложению"));
    ui->pushButton_open->setToolTip(tr("открыть файл для редактирования"));
    ui->pushButton_open_read_only->setToolTip(tr("открыть файл только для просмотра"));
    ui->pushButton_quickeSave->setToolTip(tr("сохранить изменения в открытом файле"));
    ui->pushButton_save->setToolTip(tr("выбрать файл для сохранения изменений"));
    menuLeng->setTitle(tr("Язык"));
    menuLeng->setToolTip(tr("установить язык интерфейса"));
    menuKey->setTitle(tr("корячие клавиши"));
    menuKey->setToolTip(tr("нажмите для замены комбинации клавишь"));
    setKeySave->setText(tr("сохранить как..."));
    setKeyOpen->setText(tr("открыть новый документ"));
    setKeyQuit->setText(tr("закрыть программу"));
    setKeyClose->setText(tr("закрыть документ не сохраняя"));
    changeKeyWidjet->setPlainText(tr("нажмите CTRL + клавишу для замены"));
    showKeys->setText(tr("посмотреть горячие клавиши"));
    menuTheme->setTitle(tr("оформление"));
    lightTheme->setText(tr("светлая тема"));
    darkTheme->setText(tr("тёмная тема"));
    menuTheme->setToolTip(tr("нажмите для смены темы оформления"));
}

void textEditor::personalization()
{
    setLightTheme();

    resize(860, 480);

    currentFilePath = "";


// настраиваем окно справки

    help_widget->setWindowIcon(QIcon(":/images/icon_question.png"));

    QFile fileTXT(":/texts/help.txt");

    if(fileTXT.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&fileTXT);
        help_widget->setPlainText(readText.readAll());

        fileTXT.close();
    }


// настраиваем меню

    setRu->setText("Русский");
    setEn->setText("English");

// настраиваем окно подсказки

    changeKeyWidjet->resize(500, 100);
    changeKeyWidjet->setReadOnly(true);
    changeKeyWidjet->setWindowModality(Qt::ApplicationModal);


// создаём набор горячих клавишь
    hotKeys["save"] = Qt::Key::Key_S;
    hotKeys["open"] = Qt::Key::Key_O;
    hotKeys["close"] = Qt::Key::Key_N;
    hotKeys["quit"] = Qt::Key::Key_Q;

}

QString textEditor::hoKeyList()
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

void textEditor::on_openFiler_clicked()
{
    fileView->show();
}


void textEditor::filerReturnPath(const QString& path)
{
    QFile openFile(path);
    if(openFile.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&openFile);

        ui->plainTextEdit->setPlainText(readText.readAll());

        openFile.close();

        currentFilePath = path;
        setStatusTip(path);
        ui->plainTextEdit->setReadOnly(false);
    }
}

