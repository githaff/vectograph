#include "svg_reader.h"
#include "svgobjects.h"

QMap<QString ,obj_func >* tag_factory()
{

    QMap<QString ,obj_func >* tags = new QMap<QString ,obj_func >;

    (*tags)[QString("ellipse")] = obj_func([]{return new Ellipse;});
    (*tags)[QString("circle")] = obj_func([]{return new Circle;});
    (*tags)[QString("rect")] = obj_func([]{return new Rectangle;});
    (*tags)[QString("line")] = obj_func([]{return new Line;});
    (*tags)[QString("polyline")] = obj_func([]{return new Polyline;});
    (*tags)[QString("polygon")] = obj_func([]{return new Polygon;});
    (*tags)[QString("path")] = obj_func([]{return new Path;});
    (*tags)[QString("text")] = obj_func([]{return new Text;});
    return tags;
}



void parse_svg(QString& fname,QMap<QString ,obj_func >& tags,QGraphicsScene* scene)
{

    QFile* file = new QFile(fname);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
        throw new MyExcept("can't open file");
    //класс для парсинга xml файлов
    QXmlStreamReader xml(file);
    //не обробатывать пространства имён
    xml.setNamespaceProcessing(false);
    QString name;

    //цикл по документу
    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        name = refToString(xml.name());
        if(token == QXmlStreamReader::StartElement)
        {
            if( tags.contains(name))
            {

                QGraphicsItem* item = tags.value(name)();
                Object* object = dynamic_cast<Object*>(item);
                //собрать атрибуты фигуры
                object->readFromXml(xml);
                //текст тега
                object->setText( xml.readElementText() );
                //транслировать Qt объект из SVG атрибутов
                object->transformToScene();

                //добавляем объекс на сцену
                scene->addItem(item);
            }
        }
    }
}

//открытие файла и отображение содержимого на view
bool loadSvgFileToScene(QString& fname,QMap<QString ,obj_func >& tags,QGraphicsScene* scene)
{
    //помещаем в сцену объекты из парсера
    try
    {
        parse_svg(fname,tags,scene);
    }catch(exception* e){return false;}
    return true;
}
