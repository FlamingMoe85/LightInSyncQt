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

#include "../../Ui/ShadeWidget.h"

#define AMT_DEVICES 6
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

    RGBWA_UV_MiniMovingHead* movingHead[AMT_DEVICES];
    ColorWheelMapper colWheel[AMT_DEVICES];

    ClientServer_Top cT, shiftTop, spanMinTop, spanMaxTop, spanOffsetTop, shiftSpeed;

    QTimer timer;
    std::vector<uint8_t*> universum;
    uint8_t buf[UNIV_LENGTH];
    int itteration;
    int dir;

    ShadeWidget *m_red_shade, *panShade;

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

    void pointsUpdated();
    void Slot_MouseRelease();
    void Slot_PanRelease();
};
#endif // WIDGET_H
