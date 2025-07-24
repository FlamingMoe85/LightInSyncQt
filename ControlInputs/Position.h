#ifndef POSITION_H
#define POSITION_H

#include <QWidget>
#include <QString>
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

private:
    Ui::Position *ui;

};

#endif // POSITION_H
