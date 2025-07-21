#ifndef POSITION_H
#define POSITION_H

#include <QWidget>
#include "../ClientServer_Top.h"
#include "I_Ping.hpp"


#include "../../share/BrokerClientServer/ClientServerBase.hpp"

namespace Ui {
class Position;
}

class Position : public QWidget, public I_Ping
{
    Q_OBJECT

public:
    explicit Position(QWidget *parent = nullptr, ClientServerBase *client = nullptr, QString name = " ");
    ~Position();

    void Ping(int itteration) override;

    I_Server* GetServer(){return &ctPosition;}

private:
    Ui::Position *ui;
    ClientServer_Top ctPosition;

private slots:
    void Slot_GetValue(ClientServer_Top *b, int itterration);

};

#endif // POSITION_H
