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

#include "../Ui/ShadeWidget.h"
#include "../Ui/CustomScrollArea.h"
#include "../Ui/EffectEditor.h"

#include "Audio/AudioPlayer.h"
#include "Audio/AudioPlayerFrontend.h"

#define AMT_DEVICES 1
#define UNIV_LENGTH 1 + (AMT_DEVICES*10)

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
                            bsmWheel;

    BundleSeries bsmMaster;

    MovingHead_RGBWA_UV* movingHead[AMT_DEVICES];
    ColorWheelMapper colWheel[AMT_DEVICES];

    ClientServer_Top cT, shiftTop, spanMinTop, spanMaxTop, spanOffsetTop, shiftSpeed;

    QTimer timer;
    std::vector<uint8_t*> universum;
    uint8_t buf[UNIV_LENGTH];
    int itteration;
    int dir;

    //ShadeWidget *m_red_shade, *panShade;
    //CustomScrollArea cusScrollAr;
    EffectEditor effectEditor;

    AudioPlayer audioPlayer;
    AudioPlayerFrontend audioPlayerFrontEnd;

private slots:
    void Slot_SendMsg();
    void Slot_TimerExpired();
    void Slot_GetValue(ClientServer_Top *b, int itterration);
    void Slot_GetShift(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMin(ClientServer_Top *b, int itterration);
    void Slot_GetSpanMax(ClientServer_Top *b, int itterration);
    void Slot_GetSpanOffset(ClientServer_Top *b, int itterration);
    void Slot_ShiftSpeed(ClientServer_Top *b, int itterration);

    float GetSpanMin();
    float GetSpanMax();
    float GetSpanOffset();

    void Slot_PrvStackedIndex();
    void Slot_NxtStackedIndex();
};
#endif // WIDGET_H
