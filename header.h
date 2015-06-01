#ifndef HEADER_H
#define HEADER_H

#include <QCoreApplication>
#include <QtXml>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <iostream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringRef>
#include <QMap>
#include <functional>
#include <algorithm>
#include <map>
#include <QList>
#include <QPointer>
#include <typeinfo>
#include <iterator>
#include <utility>
#include <memory>
#include <type_traits>
#include <exception>
#include <QRegExp>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QAbstractGraphicsShapeItem>
#include <QVector>
#include <QVector2D>
#include <QMoveEvent>

using namespace std;

template<typename T> using c_ref = const T&;
template<typename T> using c_ptr = const T*;


class MyExcept : public exception
{
    const char* _e;
public:
    MyExcept(const char* e){_e = e;}
    ~MyExcept(){if(_e) delete _e;}
    const char* what()const noexcept override {return _e;}
};


void correctRectPoints(QPointF& p1,QPointF& p2);


#endif // HEADER_H

