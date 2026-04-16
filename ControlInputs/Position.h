#ifndef POSITION_H
#define POSITION_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QSlider>
#include "ChangeSliderAtRate.h"
#include "../ClientServer_Top.h"
#include "I_Ping.hpp"
#include "../../../share/EffectStructureBlocks/BundleSeries.h"
#include "../../../share/EffectStructureBlocks/I_BundleSeries_UI.hpp"


#include "../../share/BrokerClientServer/ClientServerBase.hpp"

typedef struct{
    QString name;
    Qt::CheckState overridePos;
    bool enableShift;
    bool enableSpan;
    bool enableSpeed;
}PositionInit_t;

namespace Ui {
class Position;
}

class Position : public QWidget, public I_Ping, public I_BundleSeries_UI
{
    Q_OBJECT

public:
    explicit Position(QWidget *parent = nullptr, PositionInit_t _init = {" ", Qt::CheckState::Checked, true, true, true});
    ~Position();

    void Ping(int itteration) override;
    void PingUi(BundleSeries* bsPtr)override;
    void GetValue(float &_value)override;
    void SetFadeIn(qint64 _fadeIn);

    int AmtOfSlider(){return mySlider.count();};
    int GetSliderValue(int _sel){return mySlider[_sel]->value();};
    //void SetSliderValue(int _sel, int _value, bool _edge){sliderRates[_sel].GetRelValue(_value, _edge);}
    void Load(int _sel, int _val, qint64 fadeIn){sliderRates[_sel].Load((float) _val, fadeIn);}
    bool GetOverride();
    void SetOverride(bool _override);
    bool SupposedToSave();
    void DoSave(bool _save);


    void PingForSliderMove()
    {
      /*  for(int i=0; i< mySlider.count(); i++)
        {
            sliderRates[i].Ping();
        }*/
    }


private:
    Ui::Position *ui;
    QVector<QSlider*> mySlider;

    #define AMT_SLIDER_RATES    6
    ChangeSliderAtRate sliderRates[AMT_SLIDER_RATES];
    bool oldOverride;
    int myInst;
    void SpanMinMaxToCentreWidth(int _min, int _max, int &_centre, int &_width);
    void SpanCentreWidthToMinMax(int _centre, int _width, int &_min, int &_max);

private slots:
    void Slot_SetPosToMiddle();
    void Slot_SpeedDec();
    void Slot_SpeedInc();
    void on_horizontalSlider_SpanMin_valueChanged(int value);
    void on_horizontalSlider_SpanMax_valueChanged(int value);
    void on_horizontalSlider_SpanCenter_valueChanged(int value);
    void on_horizontalSlider_SpanWidth_valueChanged(int value);
};

#endif // POSITION_H
