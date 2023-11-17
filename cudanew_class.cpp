#define QT_NO_OPENGL_ES_2
#include "cudanew_class.h"

#include <phontan_cuda_lib.h>

//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cuda_gl_interop.h>

#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>
#include <QCursor>

cudanew_class::cudanew_class(QWidget *parent) :
    QGLWidget(parent)
{

  //  setMouseTracking(true);
    translate_z = -4.0;
//---------------------------
   axes_dat[0] = 0.0f ;
   axes_dat[1] = 0.0f;
   axes_dat[2] = 0.0f;

   axes_dat[3] = 1.0f;
   axes_dat[4] = 0.0f;
   axes_dat[5] = 0.0f;
//----------------------------
   axes_dat[6] = 0.0f ;
   axes_dat[7] = 0.0f;
   axes_dat[8] = 0.0f;

   axes_dat[9]  = 0.0f;
   axes_dat[10] = 1.0f;
   axes_dat[11] = 0.0f;
//----------------------------
   axes_dat[12] = 0.0f ;
   axes_dat[13] = 0.0f;
   axes_dat[14] = 0.0f;

   axes_dat[15] = 0.0f;
   axes_dat[16] = 0.0f;
   axes_dat[17] = 1.0f;
//----------------------------




    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,
            SLOT(updateTime_slot()));
    timer->start(10);

}


void cudanew_class::updateTime_slot(){

    time += 0.01f;
    //updateGL();
    this->update();
    //swapBuffers();

}

void cudanew_class::initializeGL(){


   // default initialization
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);
   // setAutoBufferSwap(true);

    initializeGLFunctions();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    unsigned int size = mesh_width * mesh_height * 4 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &vbo_axis);



}

void cudanew_class::resizeGL(int w, int h){
    // viewport
    window_width = w;
    window_height = h;
    glViewport(0, 0, w, h);
    // projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 10.0);
    //glOrtho(-1.0, 2.0, -2.0, 2.0, 2.0, 10.0);
    glFrustum(-1.0, 1.0, -1.0, 1.0, 3.0, 10.0);
    glTranslatef(0.0, 0.0, translate_z);


}



void cudanew_class::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cudaUpdateBuff(&cuda_vbo_resource, cudaGraphicsMapFlagsWriteDiscard, &vbo,
                   mesh_width, mesh_height, time);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(4, GL_FLOAT, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(1.0, 0.0, 0.0);
    glDrawArrays(GL_POINTS, 0, mesh_width * mesh_height);
    glDisableClientState(GL_VERTEX_ARRAY);

//    //==================================================

    glBindBuffer(GL_ARRAY_BUFFER, vbo_axis);
    glBufferData(GL_ARRAY_BUFFER,
                 18*sizeof(float), axes_dat,
                 GL_STATIC_DRAW);


    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINES,0,18);
    glDisableClientState(GL_VERTEX_ARRAY);


    //==================================================

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translate_x, translate_y, translate_z);
    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);

}


void cudanew_class::deleteVB0(GLuint *vbo, cudaGraphicsResource *vbo_res){

//    cudaUnregBuffer(vbo_res);
//    glBindBuffer(1, *vbo);
//    glDeleteBuffers(1, vbo);

//    *vbo = 0;
}


void cudanew_class::mousePressEvent(QMouseEvent *pe){
    m_lastPos = pe->pos();
    qDebug()<<"m_lastPos: " << pe->pos();
}

void cudanew_class::mouseMoveEvent(QMouseEvent *pe){
    int dx = pe->x() - m_lastPos.x();
    int dy = pe->y() - m_lastPos.y();

    qDebug()<< "curr_x: " << pe->x() << " dx: " << dx;
    qDebug()<< "curr_y: " << pe->y() << " dy: " << dy;

    setXRotation_slot(m_xRotate + dy);
    setYRotation_slot(m_yRotate + dx);
    m_lastPos = pe->pos();


}

void cudanew_class::keyPressEvent(QKeyEvent *key_event){
    qDebug()<< "key_event";
}

void cudanew_class::keyPress_slot(QKeyEvent *key_event){
    qDebug()<<"key press slot";
    //m_yRotate = m_yRotate + 3;
    //emit yRotationChanged(angle);
    switch(key_event->key()){
        case   Qt::Key_W:
            translate_z = translate_z + 0.1;
            break;
        case   Qt::Key_S:
            translate_z = translate_z - 0.1;
             break;
        case   Qt::Key_A:
            translate_x = translate_x + 0.05;
            break;
         case   Qt::Key_D:
            translate_x = translate_x - 0.05;
            break;

    }



    updateGL();
}


void cudanew_class::enterEvent(QEvent *enter_event){
    qDebug()<<QCursor::pos();
}


void cudanew_class::setXRotation_slot(int angle){

    normalizeAngle(&angle);
    qDebug()<<"angle from xRot_slot:" << angle;
    m_xRotate = angle;
   // emit xRotationChanged(angle);
    qDebug()<<"m_xRotate from xRot_slot:" << m_xRotate;

    updateGL();
}

void cudanew_class::setYRotation_slot(int angle){
    normalizeAngle(&angle);
    if(angle != m_yRotate){
        m_yRotate = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void cudanew_class::setZRotation_slot(int angle){
    normalizeAngle(&angle);
    if(angle != m_zRotate){
        m_zRotate = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void cudanew_class::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360;
    while (*angle > 360)
        *angle -= 360;
}
