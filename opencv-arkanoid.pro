OPENCV_SOURCES = vision/cameracapturer.cpp

OPENCV_HEADERS = vision/cameracapturer.h

TEMPLATE = app

QT += qml quick core

SOURCES += main.cpp \
  $$OPENCV_SOURCES \
    vision/cameraimageprovider.cpp \
    vision/squintdetector.cpp \
    arkanoidmanager.cpp

HEADERS += $$OPENCV_HEADERS \
    vision/cameraimageprovider.h \
    vision/squintdetector.h \
    arkanoidmanager.h

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

unix:INCLUDEPATH += "/opt/local/include"

unix:LIBPATH += "/opt/local/lib"

unix:LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui \
  -lopencv_objdetect -lopencv_video

CONFIG += c++11
