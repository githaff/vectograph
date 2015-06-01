#include <QPalette>
#include "colorselect.h"
#include "ui_colorselect.h"

ColorSelect::ColorSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSelect)
{
    ui->setupUi(this);

    connect(ui->redSlider,SIGNAL(valueChanged(int)),SLOT(onColorChanged()));
    connect(ui->greenSlider,SIGNAL(valueChanged(int)),SLOT(onColorChanged()));
    connect(ui->blueSlider,SIGNAL(valueChanged(int)),SLOT(onColorChanged()));
    onColorChanged();
}

ColorSelect::~ColorSelect()
{
    delete ui;
}

void ColorSelect::onColorChanged()
{
    m_color.setRgb(ui->redSlider->value(),ui->greenSlider->value(),ui->blueSlider->value());
    QPalette pal = ui->displayWidget->palette();
    pal.setColor(QPalette::Window,m_color);
    ui->displayWidget->setPalette(pal);

    emit colorChanged(m_color);
}
