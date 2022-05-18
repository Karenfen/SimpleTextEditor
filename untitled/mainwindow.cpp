#include "mainwindow.h"
#include "gelement.h"
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QApplication>
#include <QScreen>
#include <QGraphicsRectItem>

Figure Gelement::figure = STAR;


MainWindow::MainWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    gscene = std::make_shared<QGraphicsScene>();

    auto Wsize = qApp->primaryScreen()->size();
    Wsize.setWidth(Wsize.width() / 2);
    Wsize.setHeight(Wsize.height() / 2);

    setScene(gscene.get());

    setFixedSize(Wsize);
    gscene->setSceneRect(this->rect());

    //setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::HighQualityAntialiasing);
}

MainWindow::~MainWindow()
{
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        //Gscene->addRect(event->x(), event->y(), 50, 40, QPen(Qt::PenStyle::SolidLine), getBrush());
        //QGraphicsEllipseItem* elips = new QGraphicsEllipseItem(event->x(), event->y(), 50, 40);
        //elips->setBrush(getBrush());
        auto* elips = new Gelement;
        elips->setPos(event->x()/2 - elips->width()/4, event->y()/2 - elips->height()/4);
        gscene->addItem(elips);
        //update();
    }

}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    gscene->setSceneRect(rect());
//    fitInView(sceneRect());
//    QGraphicsView::resizeEvent(event);
//}


