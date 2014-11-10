!include("../apkstudio.pri") {
    error("Unable to locate &/or include apkstudio.pri")
}

# Enable support for newer c++ expressions
CONFIG += c++11

# Refrence QT framework dependencies
QT += core gui widgets

win32 {
    QT += winextras
}

# Source files to be included/compiled within target
HEADERS += \
    vpz/apkstudio/windows/ide.hpp \
    vpz/apkstudio/windows/chmod.hpp \
    vpz/apkstudio/windows/settings.hpp \
    vpz/apkstudio/windows/logcat.hpp \
    vpz/apkstudio/windows/information.hpp \
    vpz/apkstudio/windows/explorer.hpp \
    vpz/apkstudio/windows/dialog.hpp \
    vpz/apkstudio/windows/application.hpp \
    vpz/apkstudio/windows/file.hpp

SOURCES += \
    vpz/apkstudio/windows/ide.cpp \
    vpz/apkstudio/windows/chmod.cpp \
    vpz/apkstudio/windows/settings.cpp \
    vpz/apkstudio/windows/logcat.cpp \
    vpz/apkstudio/windows/information.cpp \
    vpz/apkstudio/windows/explorer.cpp \
    vpz/apkstudio/windows/dialog.cpp \
    vpz/apkstudio/windows/application.cpp \
    vpz/apkstudio/windows/file.cpp

# Include referenced co-projects & libraries
win32 {
    CONFIG(release, debug|release): {
        LIBS += -L$$OUT_PWD/../async/release/ -lasync
        LIBS += -L$$OUT_PWD/../components/release/ -lcomponents
        LIBS += -L$$OUT_PWD/../helpers/release/ -lhelpers
        LIBS += -L$$OUT_PWD/../resources/release/ -lresources
    } else {
        LIBS += -L$$OUT_PWD/../async/debug/ -lasync
        LIBS += -L$$OUT_PWD/../components/debug/ -lcomponents
        LIBS += -L$$OUT_PWD/../helpers/debug/ -lhelpers
        LIBS += -L$$OUT_PWD/../resources/debug/ -lresources
    }
}

unix {
    LIBS += -L$$OUT_PWD/../async/ -lasync
    LIBS += -L$$OUT_PWD/../components/ -lcomponents
    LIBS += -L$$OUT_PWD/../helpers/ -lhelpers
    LIBS += -L$$OUT_PWD/../resources/ -lresources
}

DEPENDPATH += $$PWD/../async
DEPENDPATH += $$PWD/../components
DEPENDPATH += $$PWD/../helpers
DEPENDPATH += $$PWD/../resources

INCLUDEPATH += $$PWD/../async/vpz/apkstudio
INCLUDEPATH += $$PWD/../helpers/vpz/apkstudio
INCLUDEPATH += $$PWD/../resources/vpz/apkstudio
INCLUDEPATH += $$PWD/../components/vpz/apkstudio
