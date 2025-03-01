#include "ClientServer_Top.h"

ClientServer_Top::ClientServer_Top()
{

}

void ClientServer_Top::Consume(int& _itterationCntr, float _pos)
{

}

void ClientServer_Top::Request(int& itterration)
{
    emit RequestValue(this, itterration);
}
