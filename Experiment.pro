QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../../share/BrokerClientServer/Base_Client.cpp \
    ../../../share/BrokerClientServer/Base_Server.cpp \
    ../../../share/BrokerClientServer/ClientServerBase.cpp \
    ../../../share/EffectStructureBlocks/BundleSeries.cpp \
    ../../../share/EffectStructureBlocks/BundleSeriesManager.cpp \
    ../../../share/EffectStructureBlocks/ComponentBundle.cpp \
    ../../../share/EffectStructureBlocks/EffectComponent.cpp \
    ../../../share/Functions/FunctionContainer.cpp \
    ../../../share/Functions/FunctionContainerSpan.cpp \
    ../../../share/Functions/FunctionSection_Base.cpp \
    ../../../share/Functions/FunctionSection_Linear.cpp \
    ../../../share/Mapper/ColorWheelMapper.cpp \
    ../../../share/Mapper/HandThrough.cpp \
    ../../../share/Mapper/OneChannelMapper.cpp \
    ../../share/Mapper/HandTroughMapper.cpp \
    ../UI/ShadeWidget.cpp \
    ../UI/hoverpoints.cpp \
    ClientServer_Top.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    ../../../share/BrokerClientServer/Base_Client.h \
    ../../../share/BrokerClientServer/Base_Server.h \
    ../../../share/BrokerClientServer/ClientServerBase.hpp \
    ../../../share/BrokerClientServer/I_Client.hpp \
    ../../../share/BrokerClientServer/I_Server.hpp \
    ../../../share/Devices/Channel/DmxChannel_8Bit.hpp \
    ../../../share/Devices/DmxDevices/RGBWA_UV_MiniMovingHead.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelBlue_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelDimm_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelGreen_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelPan_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelRandom.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelRed_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelTilt_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_DmxChannelWhite_8Bit.hpp \
    ../../../share/Devices/Interfaces/I_HowMany.hpp \
    ../../../share/Devices/Interfaces/I_RGB.hpp \
    ../../../share/Devices/Interfaces/I_RGBW.hpp \
    ../../../share/Devices/Interfaces/I_RGBW_Dimm.hpp \
    ../../../share/Devices/Interfaces/I_White.hpp \
    ../../../share/EffectStructureBlocks/BundleSeries.h \
    ../../../share/EffectStructureBlocks/BundleSeriesManager.h \
    ../../../share/EffectStructureBlocks/ComponentBundle.h \
    ../../../share/EffectStructureBlocks/EffectComponent.h \
    ../../../share/Functions/FunctionContainer.h \
    ../../../share/Functions/FunctionContainerSpan.h \
    ../../../share/Functions/FunctionSection_Base.h \
    ../../../share/Functions/FunctionSection_Linear.h \
    ../../../share/Functions/I_FunctionSection.hpp \
    ../../../share/Mapper/ColorWheelMapper.h \
    ../../../share/Mapper/HandThrough.h \
    ../../../share/Mapper/OneChannelMapper.h \
    ../../share/Mapper/HandTroughMapper.h \
    ../UI/ShadeWidget.h \
    ../UI/hoverpoints.h \
    ClientServer_Top.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
