#include "svgobjects.h"


//------------------перевод объектов в формат сцены----------------//

void fillVector2DFromString(QVector<QPointF>& v,QString& s)
{
    auto list = s.split(QRegExp("\\D+"));
    //убрать все возможные пробелы
    list.removeAll("");
    v.reserve( ( list.size() / 2 ) + 1);
    for(int i = 0;i < list.size();i += 2)
    {

        QPoint p(list[i].toDouble(),list[i + 1].toDouble());
        v.push_back(p);
    }
}


QColor color(QString& color)
{
    return (!color.isEmpty()) ? QColor(color) : Qt::black;
}

double realDefaults(double attr)
{//параметры по умолчанию
 //при незаданных параметрах
 //берутся равные 1
 //пример: толщина линии
    return ( attr == 0 ) ? 1 : attr;
}

double opacity(double op)
{//по умолчанию = 1
 //в отличие от других параметров
    return ( op == 0 ) ? 1 : op;
}

void Object::setItemAttributes(QGraphicsItem* item)
{
    //разрешить перемещение объекта по сцене
    //выделять объект при его выборе
    item->setFlags(QGraphicsItem::ItemIsMovable |  QGraphicsItem::ItemIsSelectable);
    //прозрачность элемента
    item->setOpacity( opacity (_fillOpacity) );
}



void Object::setShapeItemAttributes(QAbstractGraphicsShapeItem* item)
{
    setItemAttributes(item);
    QPen pen;
    //цвет обводки
    pen.setColor(QColor(_stroke ));
    //толщина обводки
    pen.setWidthF(realDefaults(_strokeWidth));
    //цвет обводки
    item->setPen(pen);

    item->setBrush(QColor(_fill));
}

void Ellipse::transformToScene()
{
    setShapeItemAttributes(this);

    double x = _cx - _rx;
    double y = _cy - _ry;
    double width = _rx * 2;
    double height = _ry * 2;

    setRect(x,y,width,height);

}
void Circle::transformToScene()
{
    setShapeItemAttributes(this);

    double x = _cx - _r;
    double y = _cy - _r;
    double diameter = _r * 2;

    setRect(x,y,diameter,diameter);
}
void Rectangle::transformToScene()
{
    setShapeItemAttributes(this);
    setRect(_x,_y,_width,_height);
}

void Line::transformToScene()
{
   setItemAttributes(this);

   QPen pen;
   pen.setColor(QColor(_stroke ));
   //толщина обводки
   pen.setWidthF(realDefaults(_strokeWidth));
   //цвет обводки
   setPen(pen);

   setLine(_x1,_y1,_x2,_y2);
}

void Polyline::transformToScene()
{
    setShapeItemAttributes(this);

    QPainterPath path;
    //начинаем рисовать с 1-вой точки
    path.moveTo(_points[0]);

    for(auto &p : _points)
        path.lineTo(p);
    setPath(path);
}

void Polygon::transformToScene()
{
    setShapeItemAttributes(this);
    setPolygon(QPolygonF(_points));
}


void Text::transformToScene()
{
    setItemAttributes(this);
    setPlainText(_text);
    //сделать текст редактируемым
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setPos(_x,_y);
}





//--------------------------перевод объектов в формат svg------------//

void Object::getItemAttributes(QGraphicsItem *item)
{
    //прозрачность
    _fillOpacity = item->opacity();
}
void Object::getShapeItemAttributes(QAbstractGraphicsShapeItem *item)
{
    getItemAttributes(item);

    QPen pen = item->pen();

    QColor penColor = pen.color();
    //обводка по умолчанию чёрная
    //не нужно записывать
    if(penColor != Qt::black)
        _stroke = penColor.name();
    _strokeWidth = pen.widthF();

    //по умолч. цвет заливки белый
    QBrush brush = item->brush();
    QColor brushColor = brush.color();
    if(brushColor != Qt::white)
        _fill = brushColor.name();
}

void Ellipse::transformToSvg()
{
    getShapeItemAttributes(this);

    QRectF rectangle = rect();
    QPointF point = QGraphicsItem::mapToScene(rectangle.x(),rectangle.y());

    double width = rectangle.width();
    double height = rectangle.height();
    double x = point.x();
    double y = point.y();
    _rx = width / 2;
    _cx = x + _rx;

    _ry = height / 2;
    _cy = y + _ry;
}

void Circle::transformToSvg()
{
    /*
    double x = _cx - _r;
    double y = _cy - _r;
    double diameter = _r * 2;
*/
    getShapeItemAttributes(this);

    QRectF rectangle = rect();
    QPointF point = QGraphicsItem::mapToScene(rectangle.x(),rectangle.y());

    double diametr = rectangle.width();
    double x = point.x();
    double y = point.y();
    _r = diametr / 2;
    _cx = x + _r;
    _cy = y + _r;
}

void Rectangle::transformToSvg()
{
    getShapeItemAttributes(this);
    QRectF rectangle = rect();
    QPointF point = QGraphicsItem::mapToScene(rectangle.x(),rectangle.y());
    _x = point.x();
    _y = point.y();
    _width = rectangle.width();
    _height = rectangle.height();
}

void Line::transformToSvg()
{
    getItemAttributes(this);

    QPen _pen = pen();

    QColor penColor = _pen.color();
    _stroke = penColor.name();

    _strokeWidth = _pen.widthF();

    QLineF _line = line();
    QPointF p1 = QGraphicsItem::mapToScene(_line.x1(),_line.y1());
    QPointF p2 = QGraphicsItem::mapToScene(_line.x2(),_line.y2());
    _x1 = p1.x();
    _y1 = p1.y();
    _x2 = p2.x();
    _y2 = p2.y();
}

size_t Polyline::vertcesBegin(QPainterPath& path)
{
    //находим начало обхода точек полилинии
    for(int i = path.elementCount() - 1;i >= 0;i--)
    {
        if(path.elementAt(i).isMoveTo())
            return i;
    }
    return 0;
}

void Polyline::verticesFromScene()
{
    QPainterPath path = shape();

    size_t vertBeg = vertcesBegin(path);
    int newSize = path.elementCount() - vertBeg;
    if( newSize != _points.size())
        _points.resize(newSize);
    size_t j = 0;
    for(int i = vertBeg;i < path.elementCount();i ++)
         _points[j++] = QGraphicsItem::mapToScene(path.elementAt(i).x,path.elementAt(i).y);
}

void Polyline::transformToSvg()
{
    getShapeItemAttributes(this);
    //записать массив точек
    verticesFromScene();
}

void Polygon::transformToSvg()
{
    getShapeItemAttributes(this);
    //массив точек
    //полигон = QVector<QPointF>
    QPolygonF pol = polygon();
    for(int i = 0;i < pol.size();i++)
       _points[i] = QGraphicsItem::mapToScene( pol.at(i) );
}

void Text::transformToSvg()
{
    getItemAttributes(this);
    QPointF position = scenePos();
    _x = position.x();
    _y = position.y();

    _text = toPlainText();
}
