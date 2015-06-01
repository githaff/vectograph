#include "MainWindow.h"
bool isPressEvent(QEvent *event)
{
    return ( event->type() == QEvent::MouseButtonPress ||
             event->type() == QEvent::TouchBegin
             );

}

bool isReleaseEvent(QEvent *event)
{
    return ( event->type() == QEvent::MouseButtonRelease ||
             event->type() == QEvent::TouchEnd
             );
}
bool isPressReleasedFigure(const DrawButtons &btn)
{// фигуры созд-мые одним протягиванием
    return (btn == DrawButtons::Line ||
            btn == DrawButtons::Ellipse ||
            btn == DrawButtons::Circle ||
            btn == DrawButtons::Rectangle
            );
}

template<typename T>
void MyGraphicsScene::addMyItem(T* item)
{
    QPen _pen;
    //QBrush _brush;
    item->setOpacity(_window->selectedOpacity());

    if(_window->selectedObjectPart() == ObjectPart::Stroke)
        _pen.setColor(_window->selectedColor());
    else
    {    //_brush.setColor(_window->selectedColor());
        item->setBrush(_window->selectedColor());
    }
    _pen.setWidthF(_window->selectedWidth());

    item->setPen(_pen);

    addItem(item);
}

void MyGraphicsScene::addEllipseItem(QPointF &p1, QPointF &p2)
{
    addMyItem<QGraphicsEllipseItem>(new Ellipse(p1,p2));
}

void MyGraphicsScene::addCircleItem(QPointF &p1, QPointF &p2)
{
    addMyItem<QGraphicsEllipseItem>(new Circle(p1,p2));
}
void MyGraphicsScene::addRectItem(QPointF &p1, QPointF &p2)
{
    addMyItem<QGraphicsRectItem>(new Rectangle(p1,p2));
}

void MyGraphicsScene::addLineItem(QPointF &p1, QPointF &p2)
{
    QGraphicsLineItem* item = new Line(p1,p2);
    item->setOpacity(_window->selectedOpacity());

    QPen _pen;

    _pen.setColor(_window->selectedColor());
    qreal w = _window->selectedWidth();
    _pen.setWidthF(_window->selectedWidth());
    item->setPen(_pen);
    addItem(item);
}

void MyGraphicsScene::createPressReleasedFigure(QPointF &p1, QPointF &p2)
{
    //создаём фигуру,заносим на сцену
    //фигура в зависимости от типа нажатой кнопки
    switch (_window->curDrawBtn()) {
    case DrawButtons::Line:
        addLineItem(p1,p2);
        return;
    case DrawButtons::Ellipse:
        addEllipseItem(p1,p2);
        return;
    case DrawButtons::Circle:
        addCircleItem(p1,p2);
        return;
    case DrawButtons::Rectangle:
        addRectItem(p1,p2);
        return;
    }

}

bool MyGraphicsScene::event(QEvent *event)
{//предобработка всех событий (событий нажатия на экран)
    //передаём событие стандартному обработчику
    return QGraphicsScene::event(event);
}


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->scenePos();
    if(isPressReleasedFigure(_window->curDrawBtn()))
    {
       //заносим первую точку в массив
       _vertices.push_back(pos);
    }
    //стандартный обработчик
    QGraphicsScene::mousePressEvent(mouseEvent);
}



void MyGraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
    QPointF pos = mouseEvent->scenePos();
    if(isPressReleasedFigure(_window->curDrawBtn()))
    {
        //заносим вторую точку в массив
        _vertices.push_back(pos);
        createPressReleasedFigure(_vertices[0],_vertices[1]);
        _vertices.clear();
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
/*
void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{


    QGraphicsScene::mouseMoveEvent(mouseEvent);
}
*/
