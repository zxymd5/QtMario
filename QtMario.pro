QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    Art.cpp \
    AudioEngine.cpp \
    AudioEngineImpl.cpp \
    BgLevelGenerator.cpp \
    BgRenderer.cpp \
    BulletBill.cpp \
    CoinAnim.cpp \
    Enemy.cpp \
    FireFlower.cpp \
    Fireball.cpp \
    FlowerEnemy.cpp \
    ImprovedNoise.cpp \
    Level.cpp \
    LevelGenerator.cpp \
    LevelRenderer.cpp \
    LevelScene.cpp \
    LoseScene.cpp \
    MapScene.cpp \
    Mario.cpp \
    Mushroom.cpp \
    Particle.cpp \
    Scene.cpp \
    Shell.cpp \
    SimpleAudioEngine.cpp \
    Sparkle.cpp \
    Sprite.cpp \
    SpriteTemplate.cpp \
    TitleScene.cpp \
    WinScene.cpp \
    main.cpp \
    MarioComponent.cpp

HEADERS += \
    Art.h \
    AudioEngine.h \
    AudioEngineImpl.h \
    BgLevelGenerator.h \
    BgRenderer.h \
    BulletBill.h \
    CoinAnim.h \
    Enemy.h \
    FireFlower.h \
    Fireball.h \
    FlowerEnemy.h \
    ImprovedNoise.h \
    Level.h \
    LevelGenerator.h \
    LevelRenderer.h \
    LevelScene.h \
    LoseScene.h \
    MapScene.h \
    Mario.h \
    MarioComponent.h \
    Mushroom.h \
    Particle.h \
    Scene.h \
    Shell.h \
    SimpleAudioEngine.h \
    Sparkle.h \
    Sprite.h \
    SpriteTemplate.h \
    TitleScene.h \
    WinScene.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/fmod/lib/release/ -lfmod
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/fmod/lib/debug/ -lfmod
else:unix: LIBS += -L$$PWD/fmod/lib/ -lfmod -lpthread -lrt

INCLUDEPATH += $$PWD/fmod/include
DEPENDPATH += $$PWD/fmod/include
