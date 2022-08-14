#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QQuickItem>
#include <QSqlDatabase>
#include <QDir>
#include <QSqlError>







MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      taskList(new QTableView),
      countTasks(new QLabel)
{
// подключаеи QML виджет
    quickeWidget = new QQuickWidget(QUrl("qrc:/gui.qml"));
    quickeWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);


// создаём дополнительные графические элементы
    auto layout = new QVBoxLayout{};
    auto Hlayout = new QHBoxLayout{};
    auto button = new QPushButton{"Список задач", this};

    layout->addWidget(quickeWidget);
    Hlayout->addWidget(button);
    Hlayout->addStretch();
    Hlayout->addWidget(new QLabel{"Колличество задач:", this});
    Hlayout->addWidget(countTasks);
    layout->addLayout(Hlayout);

    setLayout(layout);


// подключаем базу  данных
    model_ = new QSqlTableModel(taskList, QSqlDatabase::addDatabase("QSQLITE"));

    auto dataBase_ = model_->database();
    dataBase_.setDatabaseName(QDir::homePath() + "/task_list.db");

    if(!dataBase_.open())
        qDebug() << "Ошибка при открытии базы данных" << dataBase_.lastError();


// создаём таблицу в БД
    query = new QSqlQuery{dataBase_};

    if(!query->exec("create table if not exists tasks (id integer primary key, date text not null, task text not null, progress integer not null)"))
        qDebug() << "Ошибка создания таблици в базе данных" << query->lastError();


// настраиваем виджет таблицы
    taskList->setModel(model_);
    taskList->setMinimumSize(480, 240);

    model_->setTable("tasks");  /* устанавливаем нужную таблицу из БД */
    model_->select();

    countTasks->setNum(model_->rowCount());


// создаём соннекты
    auto root = quickeWidget->rootObject();
    if(root)
    {
        auto addButton = root->findChild<QObject*>("addButton");
        if(addButton)
            connect(addButton, SIGNAL(createdTask(QString, QString, QString)), this, SLOT(newTask(QString, QString, QString)) ); /* соединяем сигнал из Qml со слотом в mainwindow */
    }

    connect(button, &QPushButton::clicked, taskList, &QTableView::show);
}


MainWindow::~MainWindow()
{
    model_->database().close();
    delete countTasks;
    delete model_;
    delete taskList;
    delete query;
}


void MainWindow::newTask(const QString &text, const QString &date, const QString &progress)
{
    if(!query->exec("insert into tasks (date, task, progress) values ('" + date + "', '" + text + "', " + progress +  ")"))
        qDebug() << "Ошибка добавления нового задания" << query->lastError();

    model_->select();
    countTasks->setNum(model_->rowCount());
}

