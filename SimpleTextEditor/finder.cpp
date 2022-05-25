#include "finder.h"

Finder::Finder(QObject *parent) : QObject(parent)
{

}

void Finder::searchInDir(const QString& dir)
{
    QDir currentDir = QDir(dir);

    QStringList files = currentDir.entryList();

    if(files.isEmpty())
        return;

    for(const QString& file : files)
    {
        if(file.at(0) == ".")
            continue;

        QString path = currentDir.filePath(file);

        if(file.indexOf(fileName) != -1)
            emit foundFile(file, path);

        searchInDir(path);
    }
}

void Finder::doSearch(const QString &fileName)
{
    this->fileName = fileName;

    searchInDir(QDir::homePath());
}


Controller::Controller(QObject* parent) : QObject(parent)
{
    Finder *worker = new Finder;
    worker->moveToThread(&finderThread);
    connect(&finderThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::findFile, worker, &Finder::doSearch);
    connect(worker, &Finder::foundFile, this, &Controller::foundFile);

}

Controller::~Controller()
{
    if(finderThread.isRunning())
    {
        finderThread.quit();
        finderThread.wait();
    }
}


void Controller::start(const QString &file)
{
    if(finderThread.isRunning())
    {
        finderThread.quit();
        finderThread.wait();
    }

    finderThread.start();

    emit findFile(file);
}


void Controller::foundFile(const QString& fileName, const QString& filePath)
{
    emit sendResult(fileName, filePath);
}

