!include("../apkstudio.pri") {
    error("Unable to locate &/or include apkstudio.pri")
}

# Enable support for newer c++ expressions
CONFIG += c++11

# Refrence QT framework dependencies
QT += core gui widgets

# Source files to be included/compiled within target
HEADERS += \
    vpz/apkstudio/async/task.hpp \
    vpz/apkstudio/async/install.hpp \
    vpz/apkstudio/async/uninstall.hpp \
    vpz/apkstudio/async/pull.hpp \
    vpz/apkstudio/async/enable.hpp \
    vpz/apkstudio/async/copy.hpp \
    vpz/apkstudio/async/move.hpp \
    vpz/apkstudio/async/push.hpp \
    vpz/apkstudio/async/remove.hpp

SOURCES += \
    vpz/apkstudio/async/task.cpp \
    vpz/apkstudio/async/install.cpp \
    vpz/apkstudio/async/uninstall.cpp \
    vpz/apkstudio/async/pull.cpp \
    vpz/apkstudio/async/enable.cpp \
    vpz/apkstudio/async/copy.cpp \
    vpz/apkstudio/async/move.cpp \
    vpz/apkstudio/async/push.cpp \
    vpz/apkstudio/async/remove.cpp

# Include referenced co-projects & libraries
win32 {
    CONFIG(release, debug|release): {
        LIBS += -L$$OUT_PWD/../helpers/release/ -lhelpers
        LIBS += -L$$OUT_PWD/../resources/release/ -lresources
    } else {
        LIBS += -L$$OUT_PWD/../helpers/debug/ -lhelpers
        LIBS += -L$$OUT_PWD/../resources/debug/ -lresources
    }
}

unix {
    LIBS += -L$$OUT_PWD/../helpers/ -lhelpers
    LIBS += -L$$OUT_PWD/../resources/ -lresources
}

DEPENDPATH += $$PWD/../helpers
DEPENDPATH += $$PWD/../resources

INCLUDEPATH += $$PWD/../helpers/vpz/apkstudio
INCLUDEPATH += $$PWD/../resources/vpz/apkstudio
