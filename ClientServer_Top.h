#ifndef CLIENTSERVER_TOP_H
#define CLIENTSERVER_TOP_H

#include <QObject>
#include "../../share/BrokerClientServer/ClientServerBase.hpp"

class ClientServer_Top : public QObject, public ClientServerBase
{
    Q_OBJECT
public:
    ClientServer_Top();
    void Request(int& itterration)override;
    void Consume(int& _itterationCntr, float _pos) override;
    void RegisterCLient(I_Client* c){RegisterClient(c);}

signals:
    void RequestValue(ClientServer_Top* b, int itteration);

private:
    int myCntId;
};

#endif // CLIENTSERVER_TOP_H
