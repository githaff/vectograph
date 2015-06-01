#ifndef PARSER_H
#define PARSER_H

#include "header.h"
#include "svgobjects.h"

//обрабатываемые теги
using obj_func = std::function< QGraphicsItem* (void)>;

QMap<QString ,obj_func >* tag_factory();


void parse_svg(QString&,QMap<QString ,obj_func >& tags,QGraphicsScene* scene);
bool loadSvgFileToScene(QString& fname,QMap<QString ,obj_func >& tags,QGraphicsScene* scene);

#endif // PARSER_H

