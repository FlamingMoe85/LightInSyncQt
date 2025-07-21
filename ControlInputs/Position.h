#ifndef POSITION_H
#define POSITION_H

#include <QWidget>
#include "../ClientServer_Top.h"
#include "I_Ping.hpp"
#include "../../../share/EffectStructureBlocks/I_AlternateServer.hpp"


#include "../../share/BrokerClientServer/ClientServerBase.hpp"

namespace Ui {
class Position;
}

class Position : public QWidget, public I_Ping, public I_AlternateServer
{
    Q_OBJECT

public:
    explicit Position(QWidget *parent = nullptr, I_Client *client = nullptr, QString name = " ", bool _defaultCheck = false);
    ~Position();

    void Ping(int itteration) override;
    bool GetValue(float &_value) override;
    void SetValue(float _value) override;

    I_Server* GetServer(){return &ctPosition;}

private:
    Ui::Position *ui;
    ClientServer_Top ctPosition;

private slots:
    void Slot_GetValue(ClientServer_Top *b, int itterration);

};

#endif // POSITION_H
