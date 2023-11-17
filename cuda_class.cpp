#define QT_NO_OPENGL_ES_2
#include "cuda_class.h"

#include <Phontan_Cuda/phontan_cuda_lib.h>
#include <QDebug>
#include <QGLWidget>

#include <QMouseEvent>




Cuda_Class::Cuda_Class(QWidget *parent) :
    QGLWidget(parent)
{

    g_vertex_buffer_data[0] = 0.0f ;
    g_vertex_buffer_data[1] = 1.0f;
    g_vertex_buffer_data[2] = 0.0f;

    g_vertex_buffer_data[3] = -1.0f;
    g_vertex_buffer_data[4] = -1.0f;
    g_vertex_buffer_data[5] = 0.0f;

    g_vertex_buffer_data[6] = 1.0f;
    g_vertex_buffer_data[7] = -1.0f;
    g_vertex_buffer_data[8] = 0.0f;

    xRot = 0;
    yRot = 0;
    zRot = 0;
    qDebug()<<"Cuda_Class";


}


void Cuda_Class::initializeGL(){
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //m_nPyramid = createPyramid(1.2f);
}


void Cuda_Class::resizeGL(int w, int h){
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);

}

void Cuda_Class::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);

    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);

    Triangle();
}

void Cuda_Class::Triangle(){

        initializeGLFunctions();

        glGenBuffers(1, &vbo_tr);
        glBindBuffer(GL_ARRAY_BUFFER,  vbo_tr);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(g_vertex_buffer_data), NULL,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        runCuda(vbo_tr);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tr);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(4,GL_FLOAT, 0, 0);
        //glDrawArrays(GL_POINTS,0,256*256);
        glDrawArrays(GL_TRIANGLES,0,3);

        CudaDeleteVBO(vbo_tr);

        glDisableClientState(GL_VERTEX_ARRAY);


//=================================

//       glEnableClientState(GL_VERTEX_ARRAY);
//       glVertexPointer(3,GL_FLOAT, 0, 0);
//       glDrawArrays(GL_TRIANGLES,0,9);
//       glDisableClientState(GL_VERTEX_ARRAY);


//===============================
//    initializeGLFunctions();

//    glGenBuffers(1, &vbo_tr);
//    glBindBuffer(GL_ARRAY_BUFFER,  vbo_tr);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
//                 GL_STATIC_DRAW);
//   glEnableClientState(GL_VERTEX_ARRAY);
//   glVertexPointer(3,GL_FLOAT, 0, 0);
//   glDrawArrays(GL_TRIANGLES,0,3);
//   glDisableClientState(GL_VERTEX_ARRAY);

//===============================


//       glBegin(GL_TRIANGLES);
//           glVertex3f(0.0f,1.0f,0.0f);
//           glVertex3f(-1.0f,-1.0f,0.0f);
//           glVertex3f(1.0f,-1.0f,0.0f);
//       glEnd();

//================================
//       glEnableClientState(GL_VERTEX_ARRAY);

//       glVertexPointer(3,GL_FLOAT, 0, &g_vertex_buffer_data);

//       glDrawArrays(GL_TRIANGLES, 0, 3);

//       glDisableClientState(GL_VERTEX_ARRAY);
//================================



}

void Cuda_Class::CudaDeleteVBO(GLuint vbo){
    glBindBuffer(GL_ARRAY_BUFFER,  vbo_tr);
    glDeleteBuffers( 1, &vbo_tr);
    cuda_unreg_vbo(vbo);
}


void Cuda_Class::CreateVBO(){



   // cudaGLMapBufferObject(); // :)
   // cudaGLRegisterBufferObject();  // :)
   //cudaGLMapBufferObject(); // :)
}

void Cuda_Class::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    qDebug()<<"pos";
}

void Cuda_Class::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void Cuda_Class::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void Cuda_Class::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void Cuda_Class::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void Cuda_Class::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <cuda_gl_interop.h>
//#include <cuda_runtime.h>
//#include <cuda_runtime_api.h>
//#include <random>
//#include <QGLFunctions>
//#include <QtOpenGL>

//#include <QtOpenGL/qgl.h>
//#include <QtOpenGL/qglbuffer.h>
//#include <QtOpenGL/qglcolormap.h>
//#include <QtOpenGL/qglframebufferobject.h>
//#include <QtOpenGL/qglfunctions.h>
//#include <QtOpenGL/qglpixelbuffer.h>
//#include <QtOpenGL/qglshaderprogram.h>
//#include <GL/GLU.h>
