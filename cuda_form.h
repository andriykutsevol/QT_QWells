#ifndef CUDA_FORM_H
#define CUDA_FORM_H
#define QT_NO_OPENGL_ES_2
#include <QWidget>



namespace Ui {
class Cuda_Form;
}

class Cuda_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Cuda_Form(QWidget *parent = 0);
    ~Cuda_Form();

private:
    Ui::Cuda_Form *ui;
};

#endif // CUDA_FORM_H
