#include "header.h"

void correctRectPoints(QPointF& p1,QPointF& p2)
{//получение верхней левой и нижней правой точек
    //из двух произвольных
    size_t temp = 0;
    if (p1.x() > p2.x())
    {
       temp = p1.x();
       p1.setX( p2.x() );
       p2.setX( temp );
    }
    if (p1.y() > p2.y())
    {
       temp = p1.y();
       p1.setY( p2.y() );
       p2.setY( temp );
    }
}
