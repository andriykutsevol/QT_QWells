#ifndef OPENGL_FORM_H
#define OPENGL_FORM_H
#define QT_NO_OPENGL_ES_2
#include <QWidget>
#include <QGLWidget>

namespace Ui {
class OpenGL_Form;
}

class OpenGL_Form : public QWidget
{
    Q_OBJECT

public:
    explicit OpenGL_Form();
    ~OpenGL_Form();

private:
    Ui::OpenGL_Form *ui;
};

#endif // OPENGL_FORM_H
