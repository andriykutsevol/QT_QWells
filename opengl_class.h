#ifndef OPENGL_CLASS_H
#define OPENGL_CLASS_H
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>

// Создали сначала класс. Базовый класс QGLWidget.
// тип калсса производный от QWidget.
// Затем преобразовали виджет, указав имя класса OpenGL_Class
//  заголовочный файл opengl_class.h
// Вот и все, сгенерированный компилятором uic программный код будет
// содержать оператор #include "opengl_class.h"
// вместо #include "QTextEdit".
// И будет инстанцировать Blockable_TextEdit.

class OpenGL_Class : public QGLWidget
{
    Q_OBJECT

private:
    GLuint   m_nPyramid;
    GLfloat  m_xRotate;
    GLfloat  m_yRotate;
    GLfloat  m_zRotate;
    QPoint   m_lastPos;

    void normalizeAngle(int *angle);

public:
    explicit OpenGL_Class(QWidget *parent = 0);


protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    GLuint createPyramid(GLfloat fSize = 1.0f);


signals:

    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

public slots:
    void setXRotation_slot(int angle);
    void setYRotation_slot(int angle);
    void setZRotation_slot(int angle);

};

#endif // OPENGL_CLASS_H
