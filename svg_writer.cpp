#include "svg_writer.h"


void svg_writer(QString& fileName,QGraphicsScene* scene)
{
    QFile* file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
        throw new MyExcept("can't open file");
    //класс для записи xml
    QXmlStreamWriter xml(file);
    //писатель автоматически добавляет переходы на новую строку и
    //отступы между элементами
    xml.setAutoFormatting(true);
    //заголовок
    xml.writeStartDocument();
    //The dtd represents the entire doctypedecl
    //production from the XML 1.0 specification.
    const QString dtd("<!DOCTYPE svg >");
    xml.writeDTD(dtd);

    xml.writeStartElement("svg");
    xml.writeAttribute("xmlns", "http://www.w3.org/2000/svg");
    xml.writeAttribute("version", "1.1");
    QRectF scene_rect = scene->sceneRect();
    //размеры =  размерам сцены

    xml.writeAttribute("x",  QString::number(scene_rect.x()));
    xml.writeAttribute("y",  QString::number( scene_rect.y()));
    xml.writeAttribute("width", QString::number( scene_rect.width() ));
    xml.writeAttribute("height", QString::number(scene_rect.height() ));

    //записываем объекты в файл
    QList<QGraphicsItem *> items = scene->items();

    for(QGraphicsItem* pItem : items)
    {
        Object* pObj = dynamic_cast<Object*>(pItem);
        pObj->transformToSvg();
        pObj->writeToXml(xml);
    }

    xml.writeEndElement();//svg
    xml.writeEndDocument();//конец документа
    //закрыть файл
    file->close();
}














