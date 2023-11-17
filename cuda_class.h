#ifndef CUDA_CLASS_H
#define CUDA_CLASS_H
#define QT_NO_OPENGL_ES_2
#include <QGLWidget>
#include <QGLFunctions>

#include <cuda_gl_interop.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>



class Cuda_Class : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

private:
    GLuint vbo;
    GLfloat xRot, yRot, zRot;

    QPoint lastPos;

    cudaGraphicsResource *resource;

    void CreateVBO();
    void CudaDeleteVBO(GLuint vbo);
    //void runCuda();

    GLuint vbo_tr;
    GLfloat g_vertex_buffer_data[9];
    void Triangle();

    void normalizeAngle(int *angle);



public:
    explicit Cuda_Class(QWidget *parent = 0);
    int xRotation() const {return xRot;}
    int yRotation() const {return yRot;}
    int zRotation() const {return zRot;}

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // CUDA_CLASS_H
