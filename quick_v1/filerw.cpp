#include "filerw.h"
#include <QQuickItem>



FileRW::FileRW(QObject *parent) : QObject(parent)
{
    file =  new QFile("data.bin", this);
}


FileRW::~FileRW()
{
    if(file)
        file->close();
}


QList<QStringList> FileRW::read()
{
    if(!cache.isEmpty())
        return cache;

    if(!file)
        return cache;

    if(!file->open(QFile::ReadOnly))
        return cache;

    while (!file->atEnd())
    {
        QStringList taskData{};

        int lenght = 0;
        file->read((char*)&lenght, sizeof (lenght));

        for(int iter = 0; iter < lenght ; ++iter)
        {
            int size = 0;
            file->read((char*)&size, sizeof (size));

            QByteArray byteData;
            byteData.resize(size);
            file->read(byteData.data(), size);

            taskData.append(QString(byteData));
        }

        cache.append(taskData);
    }

    file->close();

    return cache;
}


void FileRW::write(const QStringList& data)
{

    if(data.isEmpty())
        return;

    if(!file)
        return;

    if(!file->open(QFile::Append))
        return;

    int lenght = data.length();

    file->write((char*)&lenght, sizeof (lenght));

    for(auto item : data)
    {
        lenght = item.toUtf8().length();
        file->write((char*)&lenght, sizeof (lenght));
        file->write(item.toUtf8(), lenght);
    }

    file->close();

    cache.append(data);
}

