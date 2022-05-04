#include "texteditor.h"
#include "ui_texteditor.h"

#include <QTextStream>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPlainTextEdit>
#include <QMdiSubWindow>
#include <QToolBar>



textEditor::textEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::textEditor)
{
    ui->setupUi(this);

    translator = new QTranslator;
    help_widget = new QPlainTextEdit;
    changeKeyWidjet = new QPlainTextEdit;
    plaintext = new QPlainTextEdit(this);
    fileView = std::make_shared<filer>(nullptr, ".txt");
    ui->dockWidget->setWidget(fileView.get());
    ui->mdiArea->addSubWindow(plaintext);




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

    help = new QAction(this);

    ui->menubar->addMenu(menuLeng);
    ui->menubar->addMenu(menuKey);
    ui->menubar->addMenu(menuTheme);
    ui->menubar->addAction(help);

// добавляем tool-bar
    QToolBar* toolbar = new QToolBar{};

    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("сохранить как..."), [this] {
        on_pushButton_save_clicked();
    });

    toolbar->addAction(style()->standardIcon(QStyle::SP_FileIcon), tr("создать новый файл"), [this] {
        plaintext = new QPlainTextEdit(this);
        ui->mdiArea->addSubWindow(plaintext);
        plaintext->showMaximized();
    });

    toolbar->addAction(QPixmap(":/images/Print.ico"), tr("печать документа"), [this] {
        textEditor::on_print_clicked();
    });

    toolbar->addSeparator();

    toolbar->addAction(style()->standardIcon(QStyle::SP_DirIcon), tr("открыть/закрыть проводник"), [this] {
        on_showFiles_clicked();
    });

    addToolBar(toolbar);

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
    connect(help, &QAction::triggered, this, &textEditor::on_pushButton_help_clicked);

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
    if(help_widget->isHidden())
        help_widget->show();
    if(help_widget->isTopLevel())
    {
        help_widget->close();
        help_widget->show();
    }
}


void textEditor::on_pushButton_save_clicked()
{
    if(ui->mdiArea->subWindowList().empty())
        return;

    plaintext = qobject_cast<QPlainTextEdit*>(ui->mdiArea->activeSubWindow()->widget());

    if(plaintext->isReadOnly())
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
    QFile openFile(fileName);

    if(openFile.open(QIODevice::WriteOnly))
    {
        QTextStream writeText(&openFile);
        QString text = plaintext->toPlainText();

        openFile.write(text.toLocal8Bit(), text.length());

        openFile.close();
        plaintext->setWindowTitle(fileName);
    }

}


void textEditor::on_pushButton_quickeSave_clicked()
{
    if(ui->mdiArea->subWindowList().empty())
        return;

    plaintext = qobject_cast<QPlainTextEdit*>(ui->mdiArea->activeSubWindow()->widget());

    if(plaintext->isReadOnly() | plaintext->windowTitle().isEmpty())
        return;

    QFile openFile(plaintext->windowTitle());

    if(openFile.open(QIODevice::WriteOnly))
    {
        QTextStream writeText(&openFile);
        QString text = plaintext->toPlainText();

        openFile.write(text.toLocal8Bit(), text.length());

        openFile.close();
    }

}


void textEditor::on_pushButton_close_clicked()
{
    if(ui->mdiArea->subWindowList().empty())
        return;

    plaintext = qobject_cast<QPlainTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    ui->mdiArea->closeActiveSubWindow();

    delete plaintext;
}


void textEditor::on_pushButton_open_read_only_clicked()
{


    QString fileName = QFileDialog::getOpenFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
    QFile openFile(fileName);
    if(openFile.open(QIODevice::ReadOnly))
    {
        plaintext = new QPlainTextEdit(this);

        QTextStream readText(&openFile);

        plaintext->setPlainText(readText.readAll());

        openFile.close();

        plaintext->setReadOnly(true);
        plaintext->setWindowTitle(fileName);
        ui->mdiArea->addSubWindow(plaintext);
        plaintext->showMaximized();
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
                if(keyEvent->key() == hotKeys.at("print"))
                {
                    on_print_clicked();
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
    fileView->retranslateUi();
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

}


void textEditor::setText()
{
    help_widget->setWindowTitle(tr("Справка"));

    ui->pushButton_close->setToolTip(tr("закрыть текущий файл без сохранения изменений"));
    ui->pushButton_open_read_only->setToolTip(tr("открыть файл только для просмотра"));
    ui->pushButton_quickeSave->setToolTip(tr("сохранить изменения в открытом файле"));
    menuLeng->setToolTip(tr("установить язык интерфейса"));
    menuKey->setToolTip(tr("нажмите для замены комбинации клавишь"));
    menuTheme->setToolTip(tr("нажмите для смены темы оформления"));
    help->setToolTip(tr("открыть текст справки по приложению"));

    menuLeng->setTitle(tr("Язык"));
    menuKey->setTitle(tr("корячие клавиши"));
    menuTheme->setTitle(tr("оформление"));

    setKeySave->setText(tr("сохранить как..."));
    setKeyOpen->setText(tr("открыть новый документ"));
    setKeyQuit->setText(tr("закрыть программу"));
    setKeyClose->setText(tr("закрыть документ не сохраняя"));
    showKeys->setText(tr("посмотреть горячие клавиши"));
    lightTheme->setText(tr("светлая тема"));
    darkTheme->setText(tr("тёмная тема"));
    help->setText(tr("справка"));
    changeKeyWidjet->setPlainText(tr("нажмите CTRL + клавишу для замены"));

}

void textEditor::personalization()
{
    setLightTheme();

    resize(860, 480);

    ui->mdiArea->setBackground(Qt::NoBrush);
    plaintext->showMaximized();

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
    changeKeyWidjet->setWindowIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));


// создаём набор горячих клавишь
    hotKeys["save"] = Qt::Key::Key_S;
    hotKeys["open"] = Qt::Key::Key_O;
    hotKeys["close"] = Qt::Key::Key_N;
    hotKeys["quit"] = Qt::Key::Key_Q;
    hotKeys["print"] = Qt::Key::Key_P;

// устанавливаем иконки для кнопок
    ui->pushButton_close->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButton_quickeSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButton_open_read_only->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
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


void textEditor::filerReturnPath(const QString& path)
{
    plaintext = new QPlainTextEdit(this);
    ui->mdiArea->addSubWindow(plaintext);
    plaintext->showMaximized();

    QFile openFile(path);
    if(openFile.open(QIODevice::ReadWrite))
    {
        QTextStream readText(&openFile);

        plaintext->setPlainText(readText.readAll());

        openFile.close();

        plaintext->setReadOnly(false);
        plaintext->setWindowTitle(path);
    }

    fileView->refresh();
}


void textEditor::on_print_clicked()
{
    if(ui->mdiArea->subWindowList().empty())
        return;

    plaintext = qobject_cast<QPlainTextEdit*>(ui->mdiArea->activeSubWindow()->widget());

    QPrinter printer;
    QPrintDialog dialog(&printer, this);

    if(dialog.exec() == QDialog::Accepted)
       plaintext->print(&printer);

}


void textEditor::on_showFiles_clicked()
{
    if(ui->dockWidget->isHidden())
        ui->dockWidget->show();
    else
        ui->dockWidget->hide();
}

