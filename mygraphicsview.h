#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include "header.h"
#include "MainWindow.h"

class MyGraphicsView : public QGraphicsView
{
    QPainter _painter;
    QVector<QPointF> _vert;
    bool _flMove;
public:
    MyGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);
    MyGraphicsView(QWidget * parent = 0);
    ~MyGraphicsView();
    void mouseMoveEvent(QMouseEvent * event)override;
    void paintEvent(QPaintEvent *event)override;
};

#endif // MYGRAPHICSVIEW_H
