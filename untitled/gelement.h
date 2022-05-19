#ifndef GELEMENT_H
#define GELEMENT_H


#include <QGraphicsItem>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>


enum Figure {RECTANGLE, ELLIPSE, STAR};


class Gelement : public QGraphicsItem
{

public:
    Gelement(QGraphicsItem* parent = nullptr);
    ~Gelement();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setBrush();
    int width();
    int height();

private:
    QPolygonF star(QPointF pos);

protected:
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
//    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QBrush brush;
    int h;
    int w;
    static Figure NextFigure;
    const Figure CurFigure;


signals:


};



#endif // GELEMENT_H
