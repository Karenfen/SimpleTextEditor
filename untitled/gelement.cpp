#include "gelement.h"
#include <QPainter>
#include <QColor>
#include <QGraphicsScene>



Gelement::Gelement(QGraphicsItem *parent) :
     QGraphicsItem(parent), CurFigure(NextFigure)
{
    if(NextFigure == RECTANGLE)
        NextFigure = ELLIPSE;
    else if (NextFigure == ELLIPSE)
        NextFigure = STAR;
    else if(NextFigure == STAR)
        NextFigure = RECTANGLE;

    setPos(0,0);

    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(QColor(rand() % 255, rand() % 255, rand() % 255));

    w = 60;
    h = 40;

    wheelPressed = false;
    moving = false;

    setAcceptDrops(true);
}

Gelement::~Gelement()
{

}

void Gelement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(brush);

    switch (CurFigure) {
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
        painter->drawPolygon(star(pos()));
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

QPolygonF Gelement::star(QPointF pos)
{
    QPolygonF star;
    star << QPoint(pos.rx()+ w/2, pos.ry())
         << QPoint(pos.rx() + w/2 +h*0.19, pos.ry()+h*0.32)
         << QPoint(pos.rx()+w/2+h*0.53, pos.ry()+h*0.39)
         << QPoint(pos.rx()+w/2+h*0.29, pos.ry()+h*0.64)
         << QPoint(pos.rx()+w/2+h*0.36, pos.ry()+h)
         << QPoint(pos.rx()+ w/2, pos.ry()+h*0.86)
         << QPoint(pos.rx()+ w/2-h*0.36, pos.ry()+h)
         << QPoint(pos.rx()+ w/2-h*0.29, pos.ry()+h*0.64)
         << QPoint(pos.rx()+ w/2-h*0.56, pos.ry()+h*0.39)
         << QPoint(pos.rx()+ w/2-h*0.19, pos.ry()+h*0.32);

    return star;
}


QRectF Gelement::boundingRect() const
{
    return QRectF(x(), y(), w, h);
}

void Gelement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::RightButton)
        scene()->removeItem(this);
    else if(event->button() == Qt::MiddleButton)
        wheelPressed = true;

}

void Gelement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(!moving)
        {
            setBrush();
            prepareGeometryChange();
        }
        moving = false;
    }
    else if(event->button() == Qt::MiddleButton)
        wheelPressed = false;
}

void Gelement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!wheelPressed)
    {
        moving = true;
        QPointF Npos = mapToScene( event->pos());
        QPointF Cpos = pos();

        Npos.setX(Npos.x() - width()/2);
        Npos.setY(Npos.y() - height()/2);
        Npos /= 2;

        setPos(Npos);

        if(!collidingItems().empty())
            setPos(Cpos);
    }
    else
    {
        setTransformOriginPoint(x()+w/2,y()+h/2);
        setRotation(rotation()+1);
    }

//void Gelement::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
//{
////    event->setDropAction(Qt::MoveAction);
////    event->accept();
//    event->acceptProposedAction();
//}

//void Gelement::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
//{
////    event->setDropAction(Qt::MoveAction);
////    event->accept();
    //    event->acceptProposedAction();
}

