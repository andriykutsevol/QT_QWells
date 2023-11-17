#define QT_NO_OPENGL_ES_2
#include "opengl_form.h"
#include "ui_opengl_form.h"

OpenGL_Form::OpenGL_Form() :
    ui(new Ui::OpenGL_Form)
{
    ui->setupUi(this);

//    ui->xSlider->setRange(0, 360 * 16);
//    ui->xSlider->setSingleStep(16);
//    ui->xSlider->setPageStep(15 * 16);
//    ui->xSlider->setTickInterval(15 * 16);
//    ui->xSlider->setTickPosition(QSlider::TicksRight);

    connect(ui->xSlider, &QSlider::valueChanged,
            ui->OGL_Widget, &OpenGL_Class::setXRotation_slot);

    connect(ui->OGL_Widget, &OpenGL_Class::xRotationChanged,
            ui->xSlider, &QSlider::setValue);


    connect(ui->ySlider, &QSlider::valueChanged,
            ui->OGL_Widget, &OpenGL_Class::setYRotation_slot);

    connect(ui->OGL_Widget, &OpenGL_Class::yRotationChanged,
            ui->ySlider, &QSlider::setValue);


}

OpenGL_Form::~OpenGL_Form()
{
    delete ui;
}
