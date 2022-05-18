#include "gelement.h"
#include <QPainter>
#include <QColor>
#include <QGraphicsScene>


Gelement::Gelement(QGraphicsItem *parent) :
     QGraphicsItem(parent)
{
    if(figure == RECTANGLE)
        figure = ELLIPSE;
    else if (figure == ELLIPSE)
        figure = STAR;
    else if(figure == STAR)
        figure = RECTANGLE;
//    figure = figure % 3;
//    figure++;


    setPos(0,0);

    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(QColor(rand() % 255, rand() % 255, rand() % 255));

    w = 60;
    h = 40;
}

Gelement::~Gelement()
{

}

void Gelement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(brush);

    switch (figure) {
    case Figure::RECTANGLE :
    {
        painter->drawRect(x(), y(), w, h);
        break;
    }
    case Figure::ELLIPSE :
    {
        painter->drawEllipse(x(), y(), w, h);
        break;
    }
    case Figure::STAR :
    {
        painter->drawText(x(), y(), "star");
        break;
    }
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)

}

void Gelement::setBrush()
{
    brush.setColor(QColor(rand() % 255, rand() % 255, rand() % 255));

}

int Gelement::width()
{
    return w;
}

int Gelement::height()
{
    return h;
}


QRectF Gelement::boundingRect() const
{
    return QRectF(x(), y(), w, h);
}

void Gelement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        setBrush();
    else if(event->button() == Qt::RightButton)
        scene()->removeItem(this);

    prepareGeometryChange();
}
