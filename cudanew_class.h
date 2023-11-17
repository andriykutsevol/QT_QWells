#ifndef CUDANEW_CLASS_H
#define CUDANEW_CLASS_H
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>
#include <QGLFunctions>

#include <cuda_gl_interop.h>
class cudanew_class : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

private:
    GLuint   m_nPyramid;
    GLfloat  m_xRotate;
    GLfloat  m_yRotate;
    GLfloat  m_zRotate;

    GLfloat translate_z;
    GLfloat translate_y;
    GLfloat translate_x;


    QPoint   m_lastPos;

    QTimer *timer;

    float time;
    int window_width;
    int window_height;

    const unsigned int mesh_width    = 256;
    const unsigned int mesh_height   = 256;

    GLuint vbo;
    GLuint vbo_axis;
    GLfloat axes_dat[18];

    struct cudaGraphicsResource *cuda_vbo_resource;
    void *d_vbo_buffer = NULL;

    void deleteVB0(GLuint *vbo, struct cudaGraphicsResource *vbo_res);

    float fSize = 1.0f;

    void normalizeAngle(int *angle);

public:
    explicit cudanew_class(QWidget *parent = 0);

    int xRotation() const {return m_xRotate;}
    int yRotation() const {return m_yRotate;}
    int zRotation() const {return m_zRotate;}

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


public slots:
    void setXRotation_slot(int angle);
    void setYRotation_slot(int angle);
    void setZRotation_slot(int angle);
    void updateTime_slot();
    void keyPress_slot(QKeyEvent *key_event);


protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *enter_event);
    void keyPressEvent(QKeyEvent *key_event);

};

#endif // CUDANEW_CLASS_H
