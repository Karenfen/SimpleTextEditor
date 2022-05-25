#ifndef FINDER_H
#define FINDER_H

#include <QObject>
#include <QDir>
#include <QThread>
#include <QListWidget>


class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);

private:
    void searchInDir(const QString& dir);

public slots:
    void doSearch(const QString& fileName);

signals:
    void foundFile(QString, QString);

private:
    QString fileName;

};


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();
    void start(const QString& file);

public slots:
    void foundFile(const QString& fileName, const QString& filePath);

signals:
    void findFile(QString);
    void sendResult(QString, QString);

private:
    QThread finderThread;

};


#endif // FINDER_H
