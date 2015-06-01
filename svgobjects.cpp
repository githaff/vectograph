#include "svgobjects.h"


QString refToString(const QStringRef& ref)
{   //возвращает полноценную строку из ссылки, с position,до ref size
    if(ref.size())
        return ref.string()->mid(ref.position(),ref.size());
    return QString();
}

double tryToGetUnusualRealAttr(QXmlStreamAttributes& attr_s,const char* name,double usualVal)
{
    return attr_s.hasAttribute("",name) ? (refToString(attr_s.value("",name)).remove(QRegExp("[px]"))).toDouble() : usualVal;
}

double tryToGetRealAttr(QXmlStreamAttributes& attr_s,const char* name)
{
    return attr_s.hasAttribute("",name) ? (refToString(attr_s.value("",name)).remove(QRegExp("[px]"))).toDouble() : 0;
}

QString tryToGetStrAttr(QXmlStreamAttributes& attr_s,const char* name)
{
    return attr_s.hasAttribute("",name) ? refToString(attr_s.value("",name)) : QString("");
}

bool tryToWriteStrAttr(QXmlStreamWriter &xml,const char* attrName,QString& attr)
{
    if(!attr.isEmpty())
    {
        xml.writeAttribute(QString(attrName),attr);
        return true;
    }
    return false;
}

bool tryToWriteUnusualRealAttr(QXmlStreamWriter &xml,const char* attrName,double attr,double usualVal)
{//атрибут должен иметь значение не равное usualVal,чтобы быть записаным
    if(attr != usualVal)
    {
        xml.writeAttribute(QString(attrName),QString::number(attr));
        return true;
    }
    return false;
}

bool tryToWriteRealAttr(QXmlStreamWriter &xml,const char* attrName,double attr)
{
    if(attr != 0)
    {
        xml.writeAttribute(QString(attrName),QString::number(attr));
        return true;
    }
    return false;
}

void writePointArray(QXmlStreamWriter &xml,const char* attrName,QVector<QPointF>& points)
{
    QString p;

    for(QPointF &point : points)
    {
        p += QString::number(point.x());
        p += ',';
        p += QString::number(point.y());
        p += ' ';
    }
    p.chop(1);
    xml.writeAttribute(attrName,p);
}


bool tryToWriteText(QXmlStreamWriter &xml,QString& s)
{
    if(!s.isEmpty())
    {
        xml.writeCharacters(s);
        return true;
    }
    return false;
}


//object

void Object::readFromXml(QXmlStreamReader &xml){

    QXmlStreamAttributes attributes = xml.attributes();

    _id = tryToGetStrAttr(attributes,"id");
    _fill = tryToGetStrAttr(attributes,"fill");
    _stroke = tryToGetStrAttr(attributes,"stroke");
    _style = tryToGetStrAttr(attributes,"style");
    _fillOpacity = tryToGetUnusualRealAttr(attributes,"fill-opacity",1);
    _strokeOpacity = tryToGetUnusualRealAttr(attributes,"stroke-opacity",1);
    _strokeWidth = tryToGetUnusualRealAttr(attributes,"stroke-width",1);
    _strokeDisharray = tryToGetStrAttr(attributes,"stroke-disharray");
    _strokeLinejoin = tryToGetStrAttr(attributes,"stroke-linejoin");
    _strokeLinecap = tryToGetStrAttr(attributes,"stroke-linecap");
    _dashOffset = tryToGetStrAttr(attributes,"dash-offset");
    _strokeMitrelimit = tryToGetStrAttr(attributes,"stroke-mitrelimit");
    _filter = tryToGetStrAttr(attributes,"filter");
    _strokeOffset = tryToGetStrAttr(attributes,"stroke-offset");
}
//запись объекта в xml
void Object::writeToXml(QXmlStreamWriter &xml)
{
   tryToWriteStrAttr(xml,"id",_id);
   tryToWriteStrAttr(xml,"fill",_fill);
   tryToWriteStrAttr(xml,"stroke",_stroke);
   tryToWriteStrAttr(xml,"style",_style);
   tryToWriteUnusualRealAttr(xml,"fill-opacity",_fillOpacity,1);
   tryToWriteUnusualRealAttr(xml,"stroke-opacity",_strokeOpacity,1);
   tryToWriteUnusualRealAttr(xml,"stroke-width",_strokeWidth,1);
   tryToWriteStrAttr(xml,"stroke-disharray",_strokeDisharray);
   tryToWriteStrAttr(xml,"stroke-linejoin",_strokeLinejoin);
   tryToWriteStrAttr(xml,"stroke-linecap",_strokeLinecap);
   tryToWriteStrAttr(xml,"stroke-dashoffset",_dashOffset);
   tryToWriteStrAttr(xml,"stroke-mitrelimit",_strokeMitrelimit);
   tryToWriteStrAttr(xml,"stroke-filter",_filter);
   tryToWriteStrAttr(xml,"stroke-strokeoffset",_strokeOffset);

   //запись текста между тэгами
   tryToWriteText(xml,_text);
}

void Object::seeData(){
   qDebug()<<"OBJECT";
   qDebug()<<"id: "<<_id;
   qDebug()<<"fill: "<<_fill;
}


//ellipse

void Ellipse::readFromXml(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attributes = xml.attributes();

    Object::readFromXml(xml);

    _cx = tryToGetRealAttr(attributes,"cx");
    _cy = tryToGetRealAttr(attributes,"cy");
    _rx = tryToGetRealAttr(attributes,"rx");
    _ry = tryToGetRealAttr(attributes,"ry");
}
void Ellipse::writeToXml(QXmlStreamWriter &xml){

    xml.writeStartElement("ellipse");

    tryToWriteRealAttr(xml,"cx",_cx);
    tryToWriteRealAttr(xml,"cy",_cy);
    tryToWriteRealAttr(xml,"rx",_rx);
    tryToWriteRealAttr(xml,"ry",_ry);
    Object::writeToXml(xml);

    xml.writeEndElement();
}

void Ellipse::seeData(){
    Object::seeData();
    qDebug()<<"cx: "<<_cx<<"cy: "<<_cy<<"rx: "<<_rx<<"ry: "<<_ry;
}


//circle

void Circle::readFromXml(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attributes = xml.attributes();

    //читаем атрибуты общие для любой фигуры
    Object::readFromXml(xml);

    _cx = tryToGetRealAttr(attributes,"cx");
    _cy = tryToGetRealAttr(attributes,"cy");
    _r = tryToGetRealAttr(attributes,"r");
}

void Circle::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("circle");

    tryToWriteRealAttr(xml,"cx",_cx);
    tryToWriteRealAttr(xml,"cy",_cy);
    tryToWriteRealAttr(xml,"r",_r);
    Object::writeToXml(xml);

    xml.writeEndElement();
}

void Circle::seeData(){
    Object::seeData();
    qDebug()<<"cx: "<<_cx;
    qDebug()<<"cy: ";
    qDebug()<<_cy<<"r: ";
    qDebug()<<_r;
}


//rectangle

void Rectangle::readFromXml(QXmlStreamReader &xml)
{
    const QString namespaceUri = "";

    QXmlStreamAttributes attributes = xml.attributes();
    Object::readFromXml(xml);

    _x = tryToGetRealAttr(attributes,"x");
    _y = tryToGetRealAttr(attributes,"y");

    _width = tryToGetRealAttr(attributes,"width");
    _height = tryToGetRealAttr(attributes,"height");
}
void Rectangle::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("rect");

    tryToWriteRealAttr(xml,"x",_x);
    tryToWriteRealAttr(xml,"y",_y);
    tryToWriteRealAttr(xml,"rx",_rx);
    tryToWriteRealAttr(xml,"ry",_ry);
    tryToWriteRealAttr(xml,"width",_width);
    tryToWriteRealAttr(xml,"height",_height);
    Object::writeToXml(xml);

    xml.writeEndElement();
}

void Rectangle::seeData(){
    Object::seeData();
    qDebug()<<"RECTANGLE";
    qDebug()<<"width: "<<_width;
    qDebug()<<"height: "<<_height;
}


//line
void Line::readFromXml(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attributes = xml.attributes();

    Object::readFromXml(xml);

    _x1 = tryToGetRealAttr(attributes,"x1");
    _y1 = tryToGetRealAttr(attributes,"y1");
    _x2 = tryToGetRealAttr(attributes,"x2");
    _y2 = tryToGetRealAttr(attributes,"y2");
}
void Line::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("line");

    tryToWriteRealAttr(xml,"x1",_x1);
    tryToWriteRealAttr(xml,"y1",_y1);
    tryToWriteRealAttr(xml,"x2",_x2);
    tryToWriteRealAttr(xml,"y2",_y2);
    Object::writeToXml(xml);

    xml.writeEndElement();
}

//polyline

void Polyline::readFromXml(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attributes = xml.attributes();

    Object::readFromXml(xml);

    QString s_points = tryToGetStrAttr(attributes,"points");
    //заполняем вектор координат
    fillVector2DFromString(_points,s_points);
}
void   Polyline::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("polyline");

    //координаты точек записываем в проку через пробел
    //коорд одной точки через запятую
    writePointArray(xml,"points",_points);
    Object::writeToXml(xml);

    xml.writeEndElement();
}



//polygon

void Polygon::readFromXml(QXmlStreamReader &xml)
{
    const QString namespaceUri = "";

    QXmlStreamAttributes attributes = xml.attributes();
    //набор точек в строке
    QString s_points = tryToGetStrAttr(attributes,"points");
    //парсим строку
    fillVector2DFromString(_points,s_points);

}

void Polygon::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("polygon");

    writePointArray(xml,"points",_points);
    Object::writeToXml(xml);

    xml.writeEndElement();
}

//path

void Path::readFromXml(QXmlStreamReader &xml)
{

    QXmlStreamAttributes attributes = xml.attributes();

    Object::readFromXml(xml);

    _d = tryToGetStrAttr(attributes,"d");

}



//text

void Text::readFromXml(QXmlStreamReader &xml){
    QXmlStreamAttributes attributes = xml.attributes();
    Object::readFromXml(xml);

    _x = tryToGetRealAttr(attributes,"x");
    _y = tryToGetRealAttr(attributes,"y");
    _fontFamily = tryToGetStrAttr(attributes,"font-family");
    _fontSize = tryToGetStrAttr(attributes,"font-size");
    _fontStyle = tryToGetStrAttr(attributes,"font-style");
    _textDecoration = tryToGetStrAttr(attributes,"font-decoration");
}

void Text::writeToXml(QXmlStreamWriter &xml)
{
    xml.writeStartElement("text");

    tryToWriteRealAttr(xml,"x",_x);
    tryToWriteRealAttr(xml,"y",_y);
    Object::writeToXml(xml);

    xml.writeEndElement();
}


//-------------------------констукторы, фигуры создаются в редакторе--------//
template<typename T>
void rectCreatedItems(T* item,QPointF& p1,QPointF p2)
{//объекты создаваемые через описывающий прямоугольник
    //получить нужные опорные точки (верх лев, виз прав)
    correctRectPoints(p1,p2);
    qreal width = p2.x() - p1.x();
    qreal height = p2.y() - p1.y();

    item->setRect(p1.x(),p1.y(),width,height);
}

Ellipse::Ellipse(QPointF& p1,QPointF& p2) : QGraphicsEllipseItem(),Object(),_cx(0),_cy(0),_rx(0),_ry(0)
{
    //получить нужные опорные точки (верх лев, виз прав)
    rectCreatedItems(dynamic_cast<QGraphicsEllipseItem*>(this),p1,p2);
}

Circle::Circle(QPointF &p1, QPointF &p2) : QGraphicsEllipseItem(),Object(),_cx(0),_cy(0),_r(0)
{
    correctRectPoints(p1,p2);
    qreal width = p2.x() - p1.x();
    qreal height = p2.y() - p1.y();
    qreal diametr = std::min(width,height);
    setRect(p1.x(),p1.y(),diametr,diametr);
}

Rectangle::Rectangle(QPointF &p1, QPointF &p2) : QGraphicsRectItem(),Object(),_x(0),_y(0),_rx(0),_ry(0),_width(0),_height(0)
{
   rectCreatedItems(dynamic_cast<QGraphicsRectItem*>(this),p1,p2);
}
Line::Line(QPointF &p1, QPointF &p2) : QGraphicsLineItem(),Object(),_x1(0),_y1(0),_x2(0),_y2(0)
{
    setLine(p1.x(),p1.y(),p2.x(),p2.y());
}
