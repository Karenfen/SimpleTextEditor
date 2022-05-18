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

protected:
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QBrush brush;
    int h;
    int w;
    static Figure figure;


signals:


};



#endif // GELEMENT_H
