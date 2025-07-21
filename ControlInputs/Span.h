#ifndef Span_H
#define Span_H

#include <QWidget>
#include "../ClientServer_Top.h"
#include "I_Ping.hpp"


#include "../../share/EffectStructureBlocks/BundleSeries.h"

namespace Ui {
class Span;
}

class Span : public QWidget, public I_Ping
{
    Q_OBJECT

public:
    explicit Span(QWidget *parent = nullptr, BundleSeries *bs = nullptr, QString name = " ");
    ~Span();

    void Ping(int itteration) override;

private:
    Ui::Span *ui;
    ClientServer_Top ctUpperBoundary, ctLowerBoundary;

private slots:
    void Slot_GetUpperBoundary(ClientServer_Top *b, int itterration);
    void Slot_GetLowerBoundary(ClientServer_Top *b, int itterration);

};

#endif // Span_H
