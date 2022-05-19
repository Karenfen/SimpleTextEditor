#include "mainwindow.h"
#include "gelement.h"
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QApplication>
#include <QScreen>
#include <QGraphicsRectItem>

Figure Gelement::NextFigure = RECTANGLE;


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
    setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        QGraphicsRectItem item(0,0,60,40);
        item.setPos(event->x() - item.rect().width()/2, event->y() - item.rect().height()/2);

        if(gscene->collidingItems(&item).empty())
        {
            auto gitem = new Gelement;
            gitem->setPos(item.pos()/2);
            gscene->addItem(gitem);

        }
    }

    QGraphicsView::mousePressEvent(event);
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    gscene->setSceneRect(rect());
//    fitInView(sceneRect());
//    QGraphicsView::resizeEvent(event);
//}


