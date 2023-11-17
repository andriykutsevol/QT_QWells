#define QT_NO_OPENGL_ES_2
#include "opengl_class.h"
#include <QDebug>
#include <QMouseEvent>

OpenGL_Class::OpenGL_Class(QWidget *parent) :
    QGLWidget(parent)
{
    m_xRotate = 0;
    m_yRotate = 0;
}

void OpenGL_Class::initializeGL(){
    qglClearColor(Qt::black);
    //glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    m_nPyramid = createPyramid(1.2f);
}


void OpenGL_Class::resizeGL(int w, int h){
        glViewport(0, 0, (GLint)w, (GLint)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
        glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 10.0);
}

void OpenGL_Class::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(1.0, 0.0, -3.5);

    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);

    glCallList(m_nPyramid);
}

GLuint OpenGL_Class::createPyramid(GLfloat fSize){
    GLuint n = glGenLists(1);

       glNewList(n, GL_COMPILE);
           glBegin(GL_TRIANGLE_FAN);
               qglColor(Qt::green);
               glVertex3f(0.0, fSize, 0.0);
               glVertex3f(-fSize, -fSize, fSize);
               glVertex3f(fSize, -fSize, fSize);
               qglColor(Qt::yellow);
               glVertex3f(fSize, -fSize, -fSize);
               qglColor(Qt::blue);
               glVertex3f(-fSize, -fSize, -fSize);
               qglColor(Qt::white);
               glVertex3f(-fSize, -fSize, fSize);
           glEnd();

           glBegin(GL_QUADS);
               qglColor(Qt::red);
               glVertex3f(-fSize, -fSize, fSize);
               glVertex3f(fSize, -fSize, fSize);
               glVertex3f(fSize, -fSize, -fSize);
               glVertex3f(-fSize, -fSize, -fSize);
           glEnd();
       glEndList();

       return n;

}


void OpenGL_Class::mousePressEvent(QMouseEvent *pe){

    m_lastPos = pe->pos();
    qDebug()<<"m_lastPos: " << pe->pos();

}


void OpenGL_Class::mouseMoveEvent(QMouseEvent *pe){

    int dx = pe->x() - m_lastPos.x();
    int dy = pe->y() - m_lastPos.y();

    qDebug()<< "curr_x: " << pe->x() << " dx: " << dx;
    qDebug()<< "curr_y: " << pe->y() << " dy: " << dy;

    setXRotation_slot(m_xRotate + dy);
    setYRotation_slot(m_yRotate + dx);
    m_lastPos = pe->pos();


}

// Этот слот должен вызыватся как из mouseMoveEvent
// так и из connect для слайдера.
void OpenGL_Class::setXRotation_slot(int angle){

    normalizeAngle(&angle);
    qDebug()<<"angle from xRot_slot:" << angle;
    m_xRotate = angle;
    emit xRotationChanged(angle);
    qDebug()<<"m_xRotate from xRot_slot:" << m_xRotate;

    updateGL();
}

void OpenGL_Class::setYRotation_slot(int angle){
    normalizeAngle(&angle);
    if(angle != m_yRotate){
        m_yRotate = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void OpenGL_Class::setZRotation_slot(int angle){
    normalizeAngle(&angle);
    if(angle != m_zRotate){
        m_zRotate = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void OpenGL_Class::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360;
    while (*angle > 360)
        *angle -= 360;
}
