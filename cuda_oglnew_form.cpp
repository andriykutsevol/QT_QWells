#define QT_NO_OPENGL_ES_2
#include "cuda_oglnew_form.h"
#include "ui_cuda_oglnew_form.h"
#include "QDebug"
#include "cudanew_class.h"
#include <QKeyEvent>

Cuda_Oglnew_Form::Cuda_Oglnew_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cuda_Oglnew_Form)
{
    ui->setupUi(this);

    connect(ui->xSlider, &QSlider::valueChanged,
            ui->ogl_widget, &cudanew_class::setXRotation_slot);

    connect(ui->ogl_widget, &cudanew_class::xRotationChanged,
            ui->xSlider, &QSlider::setValue);

    connect(ui->ySlider, &QSlider::valueChanged,
            ui->ogl_widget, &cudanew_class::setYRotation_slot);

    connect(ui->ogl_widget, &cudanew_class::yRotationChanged,
            ui->ySlider, &QSlider::setValue);

    connect(this, &Cuda_Oglnew_Form::key_press_singnal,
            ui->ogl_widget, &cudanew_class::keyPress_slot);

}

Cuda_Oglnew_Form::~Cuda_Oglnew_Form()
{
    delete ui;
}

void Cuda_Oglnew_Form::keyPressEvent(QKeyEvent *key_event){
    emit key_press_singnal(key_event);
}
