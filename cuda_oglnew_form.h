#ifndef CUDA_OGLNEW_FORM_H
#define CUDA_OGLNEW_FORM_H
#define QT_NO_OPENGL_ES_2
#include <QWidget>
#include <QGLFunctions>

namespace Ui {
class Cuda_Oglnew_Form;
}

class Cuda_Oglnew_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Cuda_Oglnew_Form(QWidget *parent = 0);
    ~Cuda_Oglnew_Form();

private:
    Ui::Cuda_Oglnew_Form *ui;

protected:
    void keyPressEvent(QKeyEvent *key_event);

signals:
    void key_press_singnal(QKeyEvent *key_event);

};

#endif // CUDA_OGLNEW_FORM_H
