QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    file_helper.cpp \
    ir_or_rgb.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    file_helper.h \
    ir_or_rgb.h \
    mainwindow.h

FORMS += \
    mainwindow.ui



INCLUDEPATH += /usr/include/opencv4

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_dnn.so
#LIBS += /usr//usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_freetype.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_features2d.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_quality.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_video.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_plot.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_stereo.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_ml.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_hfs.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_shape.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_superres.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_saliency.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_text.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_tracking.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_dpm.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_videoio.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_aruco.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_photo.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_face.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_reg.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_hdf.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_videostab.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_optflow.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_stitching.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_datasets.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_flann.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_viz.so
LIBS += /usr/lib/jni/libopencv_java420.so

CONFIG(debug, debug|release){

}

CONFIG(release, debug|release){
LIBS += /usr//usr/lib/x
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
