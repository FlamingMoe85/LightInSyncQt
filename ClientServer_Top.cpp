#include "ClientServer_Top.h"

static int cntId = 0;

ClientServer_Top::ClientServer_Top()
{
    myCntId = cntId;
    cntId++;
}

void ClientServer_Top::Consume(int& _itterationCntr, float _pos)
{

}

void ClientServer_Top::Request(int& itterration)
{
    emit RequestValue(this, itterration);
}
