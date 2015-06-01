#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "header.h"
#include "svg_reader.h"
#include "svg_writer.h"
#include "mygraphicsview.h"

#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>

namespace Ui {
class MainWindow;
}

enum class DrawButtons{Arrow,Pensil,Line,Ellipse,Rectangle,Circle,Polyline,Polygon,None};
enum class ObjectPart{Stroke,Fill};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString& currentFile();
    //задать одинаковые размеры для сцены и view
    void synchronizeSceneAndView();
    //открытие файла через диалог с user
    void openSvgFile();
    //создание нового файла через модальное диалоговое окно
    void newSvgFile();
    void saveSvgFile();
    const DrawButtons& curDrawBtn(){return _curDrawBtn;}
    void curDrawBtn(DrawButtons& btn){_curDrawBtn = btn;}

    QColor selectedColor();
    qreal selectedOpacity();
    qreal selectedWidth();
    ObjectPart selectedObjectPart();

    void forbidSceneInteraction();
    void allowSceneInteraction();
private slots:
    void on_pbtnAppMenuActivation_clicked();

    void on_pbtnGraphicalMenuActivation_clicked();

    void on_pbtnNewFile_clicked();

    void on_pbtnOpenFile_clicked();

    void on_pbtnSaveFile_clicked();

    void on_pbtnLine_clicked();

    void on_pbtnRect_clicked();

    void on_pbtnCircle_clicked();

    void on_pbtnEllipse_clicked();

    void on_pbtnPointer_clicked();

    void on_btnColorPanelAct_clicked();

private:
    Ui::MainWindow *ui;
    //сцена
    QGraphicsScene* _scene;
    bool _applicationMenuActivationFlag;
    bool _graphicalMenuActivationFlag;
    bool _flBottomPanel;
    //имя текущего рабочего файла
    QString _currentFile;
    SaveFileThread _saveFileThread;
    //состояние нажатия кнопок рисования
    DrawButtons _curDrawBtn;
    //взаимодействуют ли объекты с польз
    bool _flInteraction;
    //действия при нажатии кнопки рис
    void drawButtonClick(DrawButtons btn);
};

//фигура рисуемыя одним жестом: нажать,провести отпустить
bool isPressReleasedFigure(const DrawButtons& btn);
//нажатие мышь,палец
bool isPressEvent(QEvent *event);
bool isReleaseEvent(QEvent *event);

class MyGraphicsScene : public QGraphicsScene
{
    MainWindow* _window;

public:
    MyGraphicsScene( const QRectF & sceneRect,MainWindow* window) : QGraphicsScene(sceneRect,window),
        _window(window){_vertices.reserve(100);}
    bool event(QEvent *event)override;

    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent )override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)override;
    template<typename T>
    void addMyItem(T* item);
    void addEllipseItem(QPointF &p1, QPointF &p2);
    void addCircleItem(QPointF &p1, QPointF &p2);
    void addRectItem(QPointF &p1, QPointF &p2);
    void addLineItem(QPointF &p1, QPointF &p2);

    void createPressReleasedFigure(QPointF& p1,QPointF& p2);
    //освободить память от QGraphicsItems
    ~MyGraphicsScene(){}
    //вершины для постоения объекта по точкам
    QVector<QPointF> _vertices;
};



#endif // MAINWINDOW_H
