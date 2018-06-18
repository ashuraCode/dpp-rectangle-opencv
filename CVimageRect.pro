#-------------------------------------------------
#
# Project created by QtCreator 2018-01-14T17:22:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CVimageRect
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        ashurawind.cpp

HEADERS += \
        ashurawind.h

FORMS += \
        ashurawind.ui


INCLUDEPATH += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/include

LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_stitching340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_videostab340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_calib3d340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_dnn340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_features2d340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_superres340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_highgui340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_videoio340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_imgcodecs340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_shape340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_photo340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_video340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_objdetect340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_imgproc340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_ml340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_flann340.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libopencv_core340.a

LIBS += -lgdi32 -lcomdlg32

LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libIlmImf.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibtiff.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibprotobuf.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibpng.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibwebp.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibjasper.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/liblibjpeg.a
LIBS += f:/Workspace/workspaceQt/opencv-3.4.0/build/install/x86/mingw/staticlib/libzlib.a

QMAKE_LFLAGS +=  -static -static-libgcc -static-libstdc++
