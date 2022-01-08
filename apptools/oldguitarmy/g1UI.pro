#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T11:22:08
#
#-------------------------------------------------


#       core gui
QT   += widgets multimedia network

android:QT += androidextras


TARGET = Guitarmy
TEMPLATE = app


QMAKE_CFLAGS +=  -fpermissive
QMAKE_CXXFLAGS +=  -fpermissive
QMAKE_LFLAGS +=  -fpermissive


win32:LIBS += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/WinMM.lib"


SOURCES += main.cpp\
        mainwindow.cpp \
    gview.cpp \
    g0/abitarray.cpp \
    g0/aconfig.cpp \
    g0/aexpimp.cpp \
    g0/afile.cpp \
    g0/ahistory.cpp \
    g0/amusic.cpp \
    g0/astreaming.cpp \
    g0/frequlate.cpp \
    g0/gmyfile.cpp \
    g0/gtpfiles.cpp \
    g0/maing0.cpp \
    g0/midifile.cpp \
    g0/scenario.cpp \
    g0/tab.cpp \
    g0/texttab.cpp \
    g0/types.cpp \
    apainter.cpp \
    athread.cpp \
    gpannel.cpp \
    mainviews.cpp \
    tabviews.cpp \
    g0/aclipboard.cpp \
    tabcommands.cpp \
    g0/wavefile.cpp \
    g0/fft.cpp \
    g0/waveanalys.cpp \
    g0/ptbfile.cpp \
    android_helper.cpp \
    centerview.cpp \
    qthelper.cpp \
    inputviews.cpp \
    midiengine.cpp \
    g0/build.cpp \
    g0/rec.cpp \
    libtim/miditopcm.cpp \
    libtim/common.c \
    libtim/instrum.c \
    libtim/instrum_dls.c \
    libtim/mix.c \
    libtim/output.c \
    libtim/playmidi.c \
    libtim/readmidi.c \
    libtim/readsbk.c \
    libtim/resample.c \
    libtim/sndfont.c \
    libtim/stream.c \
    libtim/tables.c \
    libtim/timidity.c

HEADERS  += mainwindow.h \
    gview.h \
    g0/abitarray.h \
    g0/aconfig.h \
    g0/aexpimp.h \
    g0/afile.h \
    g0/ahistory.h \
    g0/amusic.h \
    g0/astreaming.h \
    g0/frequlate.h \
    g0/gmyfile.h \
    g0/gtpfiles.h \
    g0/midifile.h \
    g0/tab.h \
    g0/texttab.h \
    g0/types.h \
    apainter.h \
    athread.h \
    g0/buildheader.h \
    gpannel.h \
    mainviews.h \
    tabviews.h \
    g0/aclipboard.h \
    tabcommands.h \
    g0/wavefile.h \
    g0/fft.h \
    g0/waveanalys.h \
    othLibs/ffft/Array.h \
    othLibs/ffft/Array.hpp \
    othLibs/ffft/def.h \
    othLibs/ffft/DynArray.hpp \
    othLibs/ffft/FFTReal.hpp \
    othLibs/ffft/FFTRealFixLen.hpp \
    othLibs/ffft/FFTRealPassDirect.hpp \
    othLibs/ffft/FFTRealPassInverse.hpp \
    othLibs/ffft/FFTRealSelect.hpp \
    othLibs/ffft/FFTRealUseTrigo.hpp \
    othLibs/ffft/OscSinCos.hpp \
    g0/ptbfile.h \
    android_helper.h \
    centerview.h \
    qthelper.h \
    inputviews.h \
    midiengine.h \
    g0/rec.h \
    libtim/common.h \
    libtim/dls_compat.h \
    libtim/dls1.h \
    libtim/dls2.h \
    libtim/filenames.h \
    libtim/instrum.h \
    libtim/instrum_dls.h \
    libtim/miditopcm.h \
    libtim/mix.h \
    libtim/options.h \
    libtim/output.h \
    libtim/playmidi.h \
    libtim/readmidi.h \
    libtim/resample.h \
    libtim/sbk.h \
    libtim/sflayer.h \
    libtim/sndfont.h \
    libtim/tables.h \
    libtim/timi_endian.h \
    libtim/timidity.h \
    libtim/timidity_internal.h



DISTFILES += \
    g0/actionPlan.txt \
    g0/todo.txt \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    othLibs/ffft/license.txt \
    othLibs/ffft/readme.txt \
    android/src/in/guitarmy/app/AndroidHelper.java

INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    icons.qrc \
    tests.qrc \
    info.qrc \
    icons2.qrc

