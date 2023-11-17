#include "form_myplot.h"
#include "ui_form_myplot.h"

Form_MyPlot::Form_MyPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_MyPlot)
{
    ui->setupUi(this);
}

Form_MyPlot::~Form_MyPlot()
{
    delete ui;
}
