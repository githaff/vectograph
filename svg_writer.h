#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include "header.h"
#include "svgobjects.h"

//запись сцены в svg файл
void svg_writer(QString& fileName,QGraphicsScene* scene);

class SaveFileThread : public QThread
{
    QString _fName;
    QGraphicsScene* _scene;
public:
    SaveFileThread(QString &fName,QGraphicsScene* scene,QObject* parent = 0) : QThread(parent),_fName(fName),
    _scene(scene){}
    SaveFileThread() :  QThread(){}
    void setFileName(QString &fName){ _fName = fName;}
    void setpScene(QGraphicsScene* scene){_scene = scene;}
    void setThreadParent(QObject* parent = 0){setParent(parent);}
    void run()
    {
        try
        {
            svg_writer(_fName,_scene);
        }catch(exception* e){qDebug()<<"can't write the file";delete e;}

    }
};



#endif // SVG_WRITER_H
