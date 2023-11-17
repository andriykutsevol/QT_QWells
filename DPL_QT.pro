#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T19:18:01
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = DPL_QT
TEMPLATE = app

INCLUDEPATH += /usr/local/cula/include
INCLUDEPATH += /media/andrey/Hlam/MathPhys/Documents/workspace/QTC/CUDA/Monte_Cuda_Lib/
INCLUDEPATH += /usr/include/GL/

INCLUDEPATH += /media/andrey/Hlam/MathPhys/Documents/workspace/QT_CUDA/DPL_QT/Phontan_Cuda/
LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -L/media/andrey/Hlam/MathPhys/Documents/workspace/QTC/CUDA/Monte_Cuda_Lib/Debug/
LIBS += -lMonte_Cuda_Lib

LIBS += -lcurand


LIBS += -L/media/andrey/Hlam/MathPhys/Documents/workspace/QT_CUDA/DPL_QT/Phontan_Cuda/Debug/
LIBS += -lPhontan_Cuda

#QWT

#QWT_LOCATION = /usr/local/qwt-6.0.0-rc5/

#INCLUDEPATH +=  $${QWT_LOCATION}/include/
#LIBS =          -L$${QWT_LOCATION}/lib \
#                -lqwt

#LIBS += -L /usr/local/cula/lib64/

LIBS += -L/usr/lib/x86_64-linux-gnu -lGLEW
LIBS += -L/usr/lib/x86_64-linux-gnu -lGL
LIBS += -L/usr/lib/x86_64-linux-gnu -lGLU
LIBS += -L/usr/lib/x86_64-linux-gnu -lglut
LIBS += -lgmp
LIBS += -lgmpxx
LIBS += -lgsl
LIBS += -lgslcblas
LIBS += -lm
LIBS += -lboost_thread
LIBS += -lboost_system
LIBS += -llapacke
LIBS += -larmadillo
LIBS += -L/usr/lib/nvidia-current -lcuda
LIBS += -lcula_lapack_basic
LIBS += -lboost_system
LIBS += -lboost_filesystem

SOURCES += main.cpp\
        mainwindow.cpp \
    myqwsystem.cpp \
    qw_input_textedit.cpp \
    get_wave_funct.cpp \
    mag_field.cpp \
    units_trans.cpp \
    Plot/qcustomplot.cpp \
    wf_plot.cpp \
    b_theta_form.cpp \
    auto_plot.cpp \
    form_myplot.cpp \
    myplot.cpp \
    opengl_form.cpp \
    opengl_class.cpp \
    cuda_form.cpp \
    cuda_class.cpp \
    cuda_oglnew_form.cpp \
    cudanew_class.cpp \
    SomeTests/some.cpp \
    SomeTests/some1.cpp




HEADERS  += mainwindow.h \
    myqwsystem.h \
    qw_input_textedit.h \
    units_trans.h \
    Plot/qcustomplot.h \
    wf_plot.h \
    b_theta_form.h \
    auto_plot.h \
    form_myplot.h \
    myplot.h \
    header.h \
    opengl_form.h \
    opengl_class.h \
    cuda_form.h \
    cuda_class.h \
    cuda_oglnew_form.h \
    cudanew_class.h \
    SomeTests/some.h

FORMS    += mainwindow.ui \
    wf_plot.ui \
    b_theta_form.ui \
    auto_plot.ui \
    form_myplot.ui \
    opengl_form.ui \
    cuda_form.ui \
    cuda_oglnew_form.ui

RESOURCES += \
    images.qrc
