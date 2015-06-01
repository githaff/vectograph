#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //сглаживание для линий
    ui->gvGraphRegion->setRenderHint(QPainter::Antialiasing,true);

    _applicationMenuActivationFlag = false;
    _graphicalMenuActivationFlag = false;

    _flBottomPanel = false;
    ui->bottomPanel->hide();

    //кнопки рисования в ненажатом состоянии
    _curDrawBtn = DrawButtons::None;
    //вкл взаимодействие объектов с польз
    _flInteraction = true;
    //ui->colorSelect->ch

}
QColor MainWindow::selectedColor()
{//цвет выбранный пользователем
    return ui->colorSelect->color();
}
qreal MainWindow::selectedOpacity()
{
    return (qreal)( ui->sliderOpacity->value() ) / (qreal)100;
}
qreal MainWindow::selectedWidth()
{
    return ui->spinBoxLineWidth->value();
}
ObjectPart MainWindow::selectedObjectPart()
{
    if(ui->sliderFillStroke->value() == 0)
        return ObjectPart::Fill;
    else return ObjectPart::Stroke;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::forbidSceneInteraction()
{
    //запретить движение item
    QList<QGraphicsItem *> items = _scene->items();
    for(QGraphicsItem * item : items)
    {

        item->setFlag(QGraphicsItem::ItemIsMovable,false);
        item->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }
}
void MainWindow::allowSceneInteraction()
{
    //запретить движение item
    QList<QGraphicsItem *> items = _scene->items();
    for(QGraphicsItem * item : items)
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

void MainWindow::synchronizeSceneAndView()
{
    QRectF viewRect = ui->gvGraphRegion->rect();
    _scene = new MyGraphicsScene(viewRect,this);

    //связываем
    ui->gvGraphRegion->setScene(_scene);
    //ui->graphicsView->setScene(_scene);

}


const QString& MainWindow::currentFile()
{
    return _currentFile;
}

void MainWindow::on_pbtnAppMenuActivation_clicked()
{
    if(!_applicationMenuActivationFlag)
    {
        ui->grpbxAppMenu->hide();
        _applicationMenuActivationFlag = true;
    }
    else
    {
        ui->grpbxAppMenu->show();
        _applicationMenuActivationFlag = false;
    }
}

void MainWindow::on_pbtnGraphicalMenuActivation_clicked()
{
    if(!_graphicalMenuActivationFlag)
    {
        ui->grpbxGraphicalMenu->hide();
        _graphicalMenuActivationFlag = true;
    }
    else
    {
        ui->grpbxGraphicalMenu->show();
        _graphicalMenuActivationFlag = false;
    }
}
void MainWindow::on_btnColorPanelAct_clicked()
{
    if(!_flBottomPanel)
    {
        ui->bottomPanel->show();
        _flBottomPanel = true;
    }
    else
    {
        ui->bottomPanel->hide();
        _flBottomPanel = false;
    }
}




void MainWindow::newSvgFile()
{
    //имя нового файла
    _currentFile = QFileDialog::getSaveFileName(this,"chose image file name","","Images(*.svg)");
    //создать svg файл
    QFile file(_currentFile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    //записать минимальную инфу в svg формате
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    const QString dtd("<!DOCTYPE svg >");
    xml.writeDTD(dtd);
    xml.writeStartElement("svg");
    xml.writeAttribute("xmlns", "http://www.w3.org/2000/svg");
    xml.writeAttribute("version", "1.1");
    xml.writeEndElement();//svg
    xml.writeEndDocument();//конец документа
    file.close();
}

void MainWindow::on_pbtnNewFile_clicked()
{
    newSvgFile();
}

void MainWindow::openSvgFile()
{
    _currentFile = QFileDialog::getOpenFileName(this,tr("open image"),"",tr("Images(*.svg)"));
     QMap<QString ,obj_func >* tag_fact = tag_factory();
    //загрузка содержимого на сцену
    loadSvgFileToScene(_currentFile,*tag_fact,_scene);
    delete tag_fact;
}


void MainWindow::on_pbtnOpenFile_clicked()
{//открытие файла
    openSvgFile();
}

void MainWindow::saveSvgFile()
{//асинхронное сохранение файла
    if(_currentFile.isEmpty())
        _currentFile = QFileDialog::getSaveFileName(this,"chose image file name","","Images(*.svg)");
    _saveFileThread.setFileName(_currentFile);
    _saveFileThread.setpScene(_scene);
    _saveFileThread.setThreadParent(this);
    //запуск потока
    _saveFileThread.start();

}

void MainWindow::drawButtonClick(DrawButtons btn)
{
    _curDrawBtn = btn;
    if(_flInteraction)
         forbidSceneInteraction();
    //выкл взаимодейств с польз
    _flInteraction = false;
}

void MainWindow::on_pbtnSaveFile_clicked()
{//сохранение файла в svg
    saveSvgFile();
}

void MainWindow::on_pbtnLine_clicked()
{
   drawButtonClick(DrawButtons::Line);
}

void MainWindow::on_pbtnRect_clicked()
{
   drawButtonClick(DrawButtons::Rectangle);
}

void MainWindow::on_pbtnCircle_clicked()
{
    drawButtonClick(DrawButtons::Circle);
}

void MainWindow::on_pbtnEllipse_clicked()
{
    drawButtonClick(DrawButtons::Ellipse);
}

void MainWindow::on_pbtnPointer_clicked()
{
    if(!_flInteraction)
        allowSceneInteraction();
    _curDrawBtn = DrawButtons::None;
    //вкл взаимодейств с польз
    _flInteraction = true;

}




