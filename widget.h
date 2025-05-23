#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QTimer>
#include <QSerialPort>

#include "ClientServer_Top.h"



#include "../../../share/EffectStructureBlocks/BundleSeriesManager.h"
#include "../../../share/Mapper/ColorWheelMapper.h"
#include "../../../share/Mapper/OneChannelMapper.h"
#include "../../../share/Mapper/HandThrough.h"
#include "../../../share/Devices/DmxDevices/RGBWA_UV_MiniMovingHead.hpp"
#include "../../../share/Devices/DmxDevices/RGBW_16B_DImm.hpp"
#include "../../../share/FunctionOwners.hpp"

#include "../Ui/ShadeWidget.h"
#include "../Ui/CustomScrollArea.h"
#include "../Ui/EffectEditor.h"

#include "Audio/AudioPlayer.h"
#include "Audio/AudioPlayerFrontend.h"

#define AMT_SECTIONS    3
#define AMT_DEVS_PER_SECTION    4
#define CHNLS_PER_CAN 8
#define UNIV_LENGTH 1 + (AMT_SECTIONS*AMT_DEVS_PER_SECTION*CHNLS_PER_CAN)

#define AMT_DEVICES 6
//#define UNIV_LENGTH 1 + (AMT_DEVICES*10)

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QSerialPort serial;

    BundleSeries bsShift;

    BundleSeriesManager
                            bundleSeriesDevice,
                            bsmPan,
                            bsmTilt,
                            bsmDimm,
                            bsmWheel,

                            bsmOfSections,
                            bsmSection[AMT_SECTIONS];

    BundleSeries bsmMaster;

    RGBWA_UV_MiniMovingHead* movingHead[AMT_DEVICES];
    ColorWheelMapper colWheel[AMT_SECTIONS*AMT_DEVS_PER_SECTION];//colWheel[AMT_DEVICES]
    RGBW_16B_Dimm* rgbw16Bdevices[AMT_SECTIONS*AMT_DEVS_PER_SECTION];

    ClientServer_Top cT, shiftSectionTop, shiftDeviceTop, spanMinTop, spanMaxTop,
                        spanOffsetTopSection[AMT_SECTIONS],
                        shiftSpeed;

    QTimer timer;
    std::vector<uint8_t*> universum;
    uint8_t buf[UNIV_LENGTH];
    int itteration;
    int dirSectionShift, dirDeviceShift;


private slots:
    void Slot_SendMsg();
    void Slot_TimerExpired();

    void Slot_GetValue(ClientServer_Top *b, int itterration);
    void Slot_GetSectionShift(ClientServer_Top *b, int itterration);
    void Slot_GetDeviceShift(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_1(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_2(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_3(ClientServer_Top *b, int itterration);
    /*
    void Slot_GetSpanMin(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMax(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffset(ClientServer_Top *b, int itterration);
    void Slot_ShiftSpeed(ClientServer_Top *b, int itterration);


    float GetSpanMin();
    float GetSpanMax();
    float GetSpanOffset();
*/

};
#endif // WIDGET_H
