#include <QtTest>
#include <QCoreApplication>
#include <QQuickItem>
#include <QTextEdit>



// add necessary includes here
#include "../quick_v1/mainwindow.h"


QStringList testData {"QML", "SpinBox:", "Binding", "loop", "detected", "for property", "\"implicitHeight\""};



class QtTest : public QObject
{
    Q_OBJECT

public:
    QtTest();
    ~QtTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_task_struct();
    void test_GUI();
    void test_speed_write();
    void reading_correctness_test();
    void test_speed_read();


private:
    MainWindow* mainwindow {};
    QQuickWidget* quickeWidget {};
    FileRW* filer {};
};

QtTest::QtTest()
{

}

QtTest::~QtTest()
{

}

void QtTest::initTestCase()
{
    mainwindow = new MainWindow;
    QVERIFY(mainwindow);

    quickeWidget = new QQuickWidget(QUrl("qrc:/gui.qml"));
    QVERIFY(quickeWidget);

    filer = new FileRW;
    QVERIFY(filer);
}

void QtTest::cleanupTestCase()
{
    filer->clear();
    auto sizeData = filer->read().size();
    QVERIFY2(!sizeData, "file isn't clear");

    delete mainwindow;
    delete quickeWidget;
    delete filer;
}

void QtTest::test_task_struct()
{
    Task t1{"text", "date", "status"};

    QCOMPARE(t1.Date(), "date");
    QCOMPARE(t1.Text(), "text");
    QCOMPARE(t1.Status(), "status");

}

void QtTest::test_GUI()
{
    auto root = quickeWidget->rootObject();
    QVERIFY(root);
    if(!root)
        QSKIP("root is invalid");

    auto textPlane = root->findChild<QObject*>("textPlane");
    QVERIFY(textPlane);
    if(!textPlane)
        QSKIP("text plane is invalid");

    textPlane->setProperty("text", "some text");
    QCOMPARE(textPlane->property("text"), "some text");



}

void QtTest::reading_correctness_test()
{
    QTest::addColumn<QStringList>("data segment");
    QTest::addColumn<QStringList>("result");

    auto data = filer->read();

    for(auto dataSegment : data)
        QTest::addRow("") << testData << dataSegment;
}

void QtTest::test_speed_write()
{
    QBENCHMARK(filer->write(testData));
}

void QtTest::test_speed_read()
{
    QBENCHMARK(filer->read());
}

QTEST_MAIN(QtTest)

#include "tst_test1.moc"
