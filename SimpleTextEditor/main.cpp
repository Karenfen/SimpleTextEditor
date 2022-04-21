#include "texteditor.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translat;
    QString language { QLocale::system().name() };

    if(language != "ru_RU")
    {
        translat.load(":/QtLanguage_en");
    }

    a.installTranslator(&translat);

    textEditor w;
    w.personalization();
    w.setText();
    w.show();
    return a.exec();
}
