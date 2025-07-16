#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QTimer>
#include <QSerialPort>

#include "ClientServer_Top.h"



#include "../../../share/EffectStructureBlocks/BundleSeriesManager.h"
#include "../../../share/Mapper/ColorWheelMapper.h"
#include "../../../share/Mapper/OneChannelMapper.h"
#include "../../../share/Devices/DmxDevices/RGBWA_UV_MiniMovingHead.hpp"
#include "../../../share/Devices/DmxDevices/MovingHead_RGBWA_UV.hpp"
#include "../../../share/FunctionOwners.hpp"

#include "../../../share/Devices/DmxDevices/Device.hpp"

#include "../Ui/ShadeWidget.h"
#include "../Ui/CustomScrollArea.h"
#include "../Ui/EffectEditor.h"

#include "Audio/AudioPlayer.h"
#include "Audio/AudioPlayerFrontend.h"

#define AMT_SECTIONS    2
#define AMT_DEVS_PER_SECTION    6
#define CHNLS_PER_CAN 8
#define UNIV_LENGTH 1 + (AMT_SECTIONS*AMT_DEVS_PER_SECTION*CHNLS_PER_CAN)

#define AMT_DEVICES 4
#define AMT_SPEAKER_HEADS 2
//#define UNIV_LENGTH 1 + (AMT_DEVICES*10)

#define UNIV_LENGTH 1 + (AMT_SECTIONS*AMT_DEVS_PER_SECTION*CHNLS_PER_CAN) + (AMT_DEVICES*10) + (AMT_SPEAKER_HEADS*10)

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

    BundleSeriesManager
                            bsmDimm,
                            bsmOfSections,
                            bsmSection[AMT_SECTIONS],
                            bsmHeads, bsmHeadsSpan,
                            bsmHeadsSpeaker, bsmHeadsSpanSpeaker;


    BundleSeries bsmMaster, bsDimm, bsMasterHeads, bsMasterHeadsSpeaker, bsHeadsPan, bsHeadsDimm,
                 bsMasterHeadsSpeakerSpeaker, bsHeadsPanSpeaker, bsHeadsDimmSpeaker;

    MovingHead_RGBWA_UV* movingHead[AMT_DEVICES];
    ColorWheelMapper colWheel[AMT_SECTIONS*AMT_DEVS_PER_SECTION];//colWheel[AMT_DEVICES]
    ColorWheelMapper colWheelHeads[AMT_DEVICES];
    Device* devices[AMT_SECTIONS*AMT_DEVS_PER_SECTION];


    MovingHead_RGBWA_UV* movingHeadSpeaker[AMT_SPEAKER_HEADS];
    ColorWheelMapper colWheelHeadsSpeaker[AMT_SPEAKER_HEADS];

    ClientServer_Top cT, shiftSectionTop, shiftDeviceTop, topCansWhite,
                        spanOffsetTopSection[AMT_SECTIONS],
                        shiftSpeed, dimmShift, dimmValueTop,spanMinTop, spanMaxTop,
                        spanMinTopHeads, spanMaxTopHeads,topHeadsColor, topHeadsMove, topHeadsDimm, topHeadsShift,
                        spanMinTopHeadsSpeaker, spanMaxTopHeadsSpeaker,topHeadsColorSpeaker, topHeadsMoveSpeaker, topHeadsDimmSpeaker;

    Device device;

    QTimer timer;
    std::vector<uint8_t*> universum;
    uint8_t buf[UNIV_LENGTH];
    int itteration;
    int dirSectionShift, dirDeviceShift, dirDimmShift, dirDimm;


private slots:
    void Slot_SendMsg();
    void Slot_TimerExpired();

    void Slot_GetValue(ClientServer_Top *b, int itterration);
    void Slot_GetCanWhite(ClientServer_Top *b, int itterration);
    void Slot_GetSectionShift(ClientServer_Top *b, int itterration);
    void Slot_GetDeviceShift(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_1(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_2(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffsetSection_3(ClientServer_Top *b, int itterration);
    void Slot_GetDimmValue(ClientServer_Top *b, int itterration);
    void Slot_GetDimmShift(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMin(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMax(ClientServer_Top *b, int itterration);

    void Slot_GetHeadColorPos(ClientServer_Top *b, int itterration);
    void Slot_GetHeadMovePos(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMinHeads(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMaxHeads(ClientServer_Top *b, int itterration);
    void Slot_GetDimmHeads(ClientServer_Top *b, int itterration);
    void Slot_GetHeadsShift(ClientServer_Top *b, int itterration);



    void Slot_GetHeadColorPosSpeaker(ClientServer_Top *b, int itterration);
    void Slot_GetHeadMovePosSpeaker(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMinHeadsSpeaker(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMaxHeadsSpeaker(ClientServer_Top *b, int itterration);
    void Slot_GetDimmHeadsSpeaker(ClientServer_Top *b, int itterration);

};
#endif // WIDGET_H
