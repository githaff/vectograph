#ifndef SVGOBJECTS_H
#define SVGOBJECTS_H

#include "header.h"


// =========================== The functions for general-purposes ===============================

QString refToString(const QStringRef& ref);

double tryToGetRealAttr(QXmlStreamAttributes& attr_s,const char* name);

QString tryToGetStrAttr(QXmlStreamAttributes& attr_s,const char* name);

void fillVector2DFromString(QVector<QPointF>& v,QString& s);
//запись непустых строковых атрибутов
bool tryToWriteStrAttr(QXmlStreamWriter &xml,const char* attrName,QString& attr);
//запись непустых числовых атрибутов
bool tryToWriteRealAttr(QXmlStreamWriter &xml,const char* attrName,double n);
//запись массива точек
void writePointArray(QXmlStreamWriter &xml,const char* attrName,QVector<QPointF>& points);
//запись текста
bool tryToWriteText(QXmlStreamWriter &xml,QString& s);
//получить верхнюю левую и нижнюю правую точки
void correctRectPoints(QPointF& p1,QPointF& p2);
// ======================== The description of the classes hierarchy ============================

class Object
{ //абстрактный базовый класс, в который нужно будет помещать все объекты в QVector

protected:


     QString _id;
     // заливка
     QString _fill;
     // обводка
     QString _stroke;
     // стиль рисунка (для более четкой визуализации)
     QString _style;
     // уровень непрозрачности области заливки от 0 до 1
     double _fillOpacity;
     // уровень непрозрачности обводки от 0 до 1
     double _strokeOpacity;
     // толщина линии обводки
     double _strokeWidth;
     // стиль линии (пунктирная, штриховая и штрих-пунктирная)
     QString _strokeDisharray;
     // вид соединений концов линий; возможны значения mitre (обычный, принятый по умолчанию), round (скругленный), bevel (рубленый).
     QString _strokeLinejoin;
     // отступ в процентах при задании линии с использованием атрибута stroke-dash
     QString _dashOffset;
     // вид концов линии; возможны значения butt (обычный, принятый по умолчанию), round (округленный) и squarel (с выступом)
     QString _strokeLinecap;
     // определяет ограничение на длину соединения двух линий, заданного с помощью атрибута stroke-linejoin="mitre"
     QString _strokeMitrelimit;
     // фильтр
     QString _filter;

     QString _strokeOffset;

     //текст находящийся в тэге
     QString _text;
public:
     Object() : _fillOpacity(1),_strokeOpacity(1),_strokeWidth(1){}
     //считывание атрибутов из xml файла
     virtual ~Object(){}

     c_ref<QString> getId()const { return _id;}
     void setId(QString& id){ _id = id; }

     c_ref<QString> getFill()const{ return _fill;}
     void setFill(QString& fill){ _fill = fill;}

     c_ref<QString> getStroke()const{ return _stroke; }
     void setStroke(QString& stroke){ _stroke = stroke;}

     c_ref<QString> getStyle()const{return _style;}
     void setStyle(QString& style){_style = style;}

     double getFillOpacity()const{ return _fillOpacity; }
     void setFillOpacity(double fillOpacity){ _fillOpacity = fillOpacity; }

     double getStrokeOpacity()const{return _strokeOpacity;}
     void setStrokeOpacity(double strokeOpacity){_strokeOpacity = strokeOpacity; }

     double getStrokeWidth()const{return _strokeWidth;}
     void setStrokeWidth(double strokeWidth){ _strokeWidth = strokeWidth;}

     c_ref<QString> getStrokeDisharray()const{return _strokeDisharray;}
     void setStrokeDisharray(QString& strokeDisharray){_strokeDisharray = strokeDisharray;}

     c_ref<QString> getStrokeLinejoin()const{return _strokeLinejoin;}
     void setStrokeLinejoin(QString& strokeLinejoin){ _strokeLinejoin = strokeLinejoin; }

     c_ref<QString> getDashOffset()const{return _dashOffset;}
     void setDashOffset(QString& dashOffset){_dashOffset = dashOffset;}

     c_ref<QString> getStrokeLinecap()const{return _strokeLinecap;}
     void setStrokeLinecap(QString& strokeLinecap){_strokeLinecap = strokeLinecap;}

     c_ref<QString> getStrokeOffset()const{return _strokeOffset;}
     void setStrokeOffset(QString& strokeOffset){_strokeOffset = strokeOffset;}

     c_ref<QString> getStrokeMitrelimit()const{return _strokeMitrelimit;}
     void setStrokeMitrelimit(QString& strokeMitrelimit){_strokeMitrelimit = strokeMitrelimit;}

     c_ref<QString> getFilter()const{return _filter;}
     void setFilter(QString& filter){_filter = filter;}


     c_ref<QString> getText()const{return _text;}
     void setText(QString text){_text = text;}

     virtual void readFromXml(QXmlStreamReader &xml);
     //запись объекта в xml
     virtual void writeToXml(QXmlStreamWriter &xml);

     virtual void seeData();
    //так как Object не является Qobject QGraphicsItem
     //(множ. наследование от Qobject запрещено)
     //то обработка атрибутов в отдельной функции
     void setItemAttributes(QGraphicsItem* item);
     void setShapeItemAttributes(QAbstractGraphicsShapeItem* item);
     //создание объектов по считанным атрибутам
     virtual void transformToScene()=0;

     void getItemAttributes(QGraphicsItem* item);
     void getShapeItemAttributes(QAbstractGraphicsShapeItem* item);
     //запись объектов в формате svg
     virtual void transformToSvg()=0;

};



// =========================== The description of the xml entities ==============================


//------------------------------------ Ellipse ---------------------------------------
class Ellipse: public QGraphicsEllipseItem,public Object
{

    double _cx;
    double _cy;
    double _rx;
    double _ry;

public:
    Ellipse() : QGraphicsEllipseItem(),Object(),_cx(0),_cy(0),_rx(0),_ry(0)
    {
        //вкл реагирование на события нажатия
         setAcceptTouchEvents(true);
    }
    Ellipse(QPointF& p1,QPointF& p2);
    ~Ellipse() {}
    double getCx()const{return _cx;}
    void setCx(double cx){_cx = cx;}

    double getCy()const{return _cy;}
    void setCy(double cy){_cy = cy;}

    double getRx()const{return _rx;}
    void setRx(double rx){_rx = rx; }

    double getRy()const{return _ry;}
    void setRy(double ry){_ry = ry; }

     friend ostream& operator<<(ostream& mystream,const Ellipse &obj);
    //void readFromXml(QXmlStreamReader &xml)override;
    void readFromXml(QXmlStreamReader &xml) override;
    void writeToXml(QXmlStreamWriter &xml)override;


    void seeData()override;

    void transformToScene()override;
    void transformToSvg()override;
};







//------------------------------------- Circle ---------------------------------------
class Circle: public QGraphicsEllipseItem,public Object
{

private:
    double _cx;
    double _cy;
    double _r;

public:
    Circle() : QGraphicsEllipseItem(),Object(),_cx(0),_cy(0),_r(0)
    {
        setAcceptTouchEvents(true);
    }
    Circle(QPointF& p1,QPointF& p2);
    double getCx()const{return _cx;}
    void setCx(double cx){_cx = cx;}

    double getCy()const{return _cy;}
    void setCy(double cy){_cy = cy;}

    double getR()const{return _r;}
    void setRx(double r){_r = r; }

    //void readFromXml(QXmlStreamReader &xml) override;
    friend ostream& operator<<(ostream& mystream, const Circle &obj);

    void readFromXml(QXmlStreamReader &xml)override;


    void writeToXml(QXmlStreamWriter &xml)override;


    void seeData()override;
    void transformToScene()override;
    void transformToSvg()override;
};





//----------------------------------- Rectangle --------------------------------------
class Rectangle: public QGraphicsRectItem,public Object
{

private:
    double _x;
    double _y;
    double _rx;
    double _ry;
    double _width;
    double _height;

public:
    Rectangle() : QGraphicsRectItem(),Object(),_x(0),_y(0),_rx(0),_ry(0),_width(0),_height(0)
    {
        setAcceptTouchEvents(true);
    }
    Rectangle(QPointF& p1,QPointF& p2);
    double getX()const{return _x;}
    void setX(double x){_x = x;}

    double getY()const{return _y;}
    void setY(double y){_y = y;}

    double getWidth() const{ return _width;}
    void setWidth(double width){_width = width;}

    double getHeight()const{return _height;}
    void setHeight(double height){_height = height;}

    //void readFromXml(QXmlStreamReader &xml)override;
    friend ostream& operator<<(ostream& mystream, const Rectangle &obj);

    void readFromXml(QXmlStreamReader &xml)override;

    void writeToXml(QXmlStreamWriter &xml)override;


    void seeData()override;

    void transformToScene()override;
    void transformToSvg()override;
};



//------------------------------------- Line -----------------------------------------
class Line: public QGraphicsLineItem,public Object
{

private:
    double _x1;
    double _y1;
    double _x2;
    double _y2;

public:
    Line() : QGraphicsLineItem(),Object(),_x1(0),_y1(0),_x2(0),_y2(0)
    {
        setAcceptTouchEvents(true);
    }
    Line(QPointF& p1,QPointF& p2);
    double getX1()const{return _x1;}
    void setX1(double x1){_x1 = x1;}

    double getY1()const{return _y1;}
    void setY1(double y1){_y1 = y1;}

    double getX2()const{return _x2;}
    void setX2(double x2){_x2 = x2;}

    double getY2()const{return _y2;}
    void setY2(double y2){_y2 = y2;}

    //void readFromXml(QXmlStreamReader &xml)override;
    friend ostream& operator<<(ostream& mystream, const Line &obj);

    void readFromXml(QXmlStreamReader &xml)override;

    void writeToXml(QXmlStreamWriter &xml)override;


    void seeData()override{}
    void transformToScene()override;
    void transformToSvg()override;
};




//----------------------------------- Polyline ---------------------------------------
class Polyline: public QGraphicsPathItem,public Object
{

private:
    QVector<QPointF> _points;

public:
    Polyline() : QGraphicsPathItem(), Object()
    {
        setAcceptTouchEvents(true);
    }

    QPointF getPoint(size_t i)const{return _points[i];}
    void setPoint(QPointF& point){_points.push_back(point);}

    //void readFromXml(QXmlStreamReader &xml)override;
    friend ostream& operator<<(ostream& mystream, const Polyline &obj);
    //начало точек обхода кривой
    size_t vertcesBegin(QPainterPath& path);

    void readFromXml(QXmlStreamReader &xml)override;

    void writeToXml(QXmlStreamWriter &xml)override;

    //получить точки кривой
    void verticesFromScene();
    void seeData()override{}
    void transformToScene()override;
    void transformToSvg()override;
};



//------------------------------------ Polygon ---------------------------------------
class Polygon: public QGraphicsPolygonItem,public Object
{

private:
    QVector<QPointF> _points;

public:
    Polygon() : QGraphicsPolygonItem(),Object()
    {
        setAcceptTouchEvents(true);
    }

    QPointF getPoint(size_t i)const{return _points[i];}
    void setPoint(size_t i,QPointF& p){_points[i] = p;}

    void readFromXml(QXmlStreamReader &xml)override;
    void writeToXml(QXmlStreamWriter &xml)override;

    void seeData()override{}
    void transformToScene()override;
    void transformToSvg()override;
};



//------------------------------------- Path ---------------------------------------
class Path : public QGraphicsPathItem,public Object
{

private:
    // набор команд для рисования элемента path
    QString _d;

public:
    Path() : QGraphicsPathItem(),Object()
    {
        setAcceptTouchEvents(true);
    }

    c_ref<QString> getD(){return _d;}
    void setD(QString& d){_d = d;}

    //void readFromXml(QXmlStreamReader &xml) override;
    friend ostream& operator<<(ostream& mystream, Path & obj);

    void readFromXml(QXmlStreamReader &xml)override;

    void writeToXml(QXmlStreamWriter &)override{}

    void seeData()override{}
    void transformToScene()override{}
    void transformToSvg()override{}
};


class Text : public QGraphicsTextItem,public Object
{

    double _x;
    double _y;
    QString _fontFamily;
    QString _fontSize;
    QString _fontStyle;
    QString _textDecoration;
public:
    Text() : QGraphicsTextItem(),Object(),_x(0),_y(0)
    {
        setAcceptTouchEvents(true);
    }

    double getX()const{return _x;}
    double getY()const{return _y;}
    c_ref<QString> getFontFamily()const{return _fontFamily;}
    c_ref<QString> getFontSize()const{return _fontSize;}
    c_ref<QString> getFontStyle()const{return _fontStyle;}
    c_ref<QString> getTextDecoration()const{return _textDecoration;}

    void setX(double x){_x = x;}
    void setY(double y){_y = y;}
    void setFontFamily(QString& fontFamily){_fontFamily = fontFamily;}
    void setFontSize(QString& fontSize){_fontSize = fontSize;}
    void setFontStyle(QString& fontStyle){_fontFamily = fontStyle;}
    void setTextDecoration(QString& textDecoration){_textDecoration = textDecoration;}



    void readFromXml(QXmlStreamReader &xml)override;

    void writeToXml(QXmlStreamWriter &xml)override;

    void seeData()override{}
    void transformToScene()override;
    void transformToSvg()override;
};




#endif // SVGOBJECTS_H

