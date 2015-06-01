#include "mygraphicsview.h"

//---------------------View---------------------//
MyGraphicsView::MyGraphicsView(QGraphicsScene * scene, QWidget * parent): QGraphicsView(scene,parent)
{
    _flMove = false;
}
MyGraphicsView::MyGraphicsView(QWidget * parent) : QGraphicsView(parent)
{
    _flMove = false;
}

MyGraphicsView::~MyGraphicsView()
{

}

void MyGraphicsView::mouseMoveEvent(QMouseEvent * event)
{

    MyGraphicsScene* myScene = dynamic_cast<MyGraphicsScene*> (scene());

    if(!myScene->_vertices.isEmpty())
    {//отображать процесс рисовани
        QPointF p1 = myScene->_vertices.front();
        //QPointF p2 = mapToScene(event->pos());
        QPointF p2 = event->pos();
        correctRectPoints(p1,p2);
        _vert.push_back(p1);
        _vert.push_back(p2);
        _flMove = true;
        viewport()->repaint(rect());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::paintEvent(QPaintEvent *event)
{
    if(_flMove)
    {
        _painter.begin(viewport());
        _painter.setPen(Qt::red);
        QPointF p1 = mapFromScene(_vert[0]);

        QPointF p2 = mapFromScene(_vert[1]);
        QRectF rect(p1.x(),p1.y(),p2.x() - p1.x(),p2.y() - p1.y());
        _painter.drawRect(rect);
        _painter.end();
        _vert.clear();
        _flMove = false;
    }
    QGraphicsView::paintEvent(event);
}
