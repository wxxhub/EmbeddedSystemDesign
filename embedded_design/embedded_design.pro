#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T09:29:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = embedded_design
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
            BOOST_USE_LIB

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /home/wxx/lib/Opencv3.4.0/include \
               /home/wxx/lib/Opencv3.4.0/include/opencv \
               /home/wxx/lib/Opencv3.4.0/include/opencv2 \

LIBS += /home/wxx/lib/Opencv3.4.0/lib/libopencv_highgui.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_core.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_imgcodecs.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_objdetect.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_imgproc.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_video.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_videoio.so \
        /usr/lib/x86_64-linux-gnu/libboost_system.a \
        /usr/lib/x86_64-linux-gnu/libboost_thread.a \


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../vision/src/vision_model.cpp \
    ../port/src/usb_port.cpp

HEADERS += \
        mainwindow.h \
    ../vision/include/vision_model.h \
    ../port/include/usb_port.h

FORMS += \
        mainwindow.ui
