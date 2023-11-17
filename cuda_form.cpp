#define QT_NO_OPENGL_ES_2
#include "cuda_form.h"
#include "ui_cuda_form.h"

Cuda_Form::Cuda_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cuda_Form)
{
    ui->setupUi(this);
}

Cuda_Form::~Cuda_Form()
{
    delete ui;
}
