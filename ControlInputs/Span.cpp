#include "Span.h"
#include "ui_Span.h"


Span::Span(QWidget *parent, BundleSeries *bs, QString name) :
    QWidget(parent),
    ui(new Ui::Span)
{
    ui->setupUi(this);
    ui->title->setText(name);
    QObject::connect(&ctUpperBoundary, &ClientServer_Top::RequestValue, this, &Span::Slot_GetUpperBoundary);
    bs->SetSerParamSpanMax(&ctUpperBoundary);
    QObject::connect(&ctLowerBoundary, &ClientServer_Top::RequestValue, this, &Span::Slot_GetLowerBoundary);
    bs->SetSerParamSpanMax(&ctLowerBoundary);
}

Span::~Span()
{
    delete ui;
}

void Span::Ping(int itteration)
{
    ctUpperBoundary.Request(itteration);
    ctLowerBoundary.Request(itteration);
}

void Span::Slot_GetUpperBoundary(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_UpperBoundary->value() / (float)ui->horizontalSlider_UpperBoundary->maximum();
    b->Serve(itterration,tmpF);
}

void Span::Slot_GetLowerBoundary(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_LowerBoundary->value() / (float)ui->horizontalSlider_LowerBoundary->maximum();
    b->Serve(itterration,tmpF);
}
