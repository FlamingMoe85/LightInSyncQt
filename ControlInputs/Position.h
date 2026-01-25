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

    int AmtOfSlider(){return mySlider.count();};
    int GetSliderValue(int _sel){return mySlider[_sel]->value();};
    //void SetSliderValue(int _sel, int _value, bool _edge){sliderRates[_sel].GetRelValue(_value, _edge);}
    void Load(int _sel, int _val){sliderRates[_sel].Load((float) _val);}
    bool GetOverride();
    void SetOverride(bool _override);
    bool SupposedToSave();
    void DoSave(bool _save);

    /**/
    void PingForSliderMove()
    {
        for(int i=0; i< mySlider.count(); i++)
        {
            sliderRates[i].Ping();
        }
    }


private:
    Ui::Position *ui;
    QVector<QSlider*> mySlider;
    ChangeSliderAtRate sliderRates[6];
    bool oldOverride;
    int myInst;

private slots:
    void Slot_SetPosToMiddle();
    void Slot_SpeedDec();
    void Slot_SpeedInc();
};

#endif // POSITION_H
