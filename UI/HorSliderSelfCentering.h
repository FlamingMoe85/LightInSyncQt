#ifndef HORSLIDERSELFCENTERING_H
#define HORSLIDERSELFCENTERING_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class HorSliderSelfCentering;
}

class HorSliderSelfCentering : public QWidget
{
    Q_OBJECT

public:
    explicit HorSliderSelfCentering(QWidget *parent = nullptr, int _min = 0, int _max = 100);
    ~HorSliderSelfCentering();
protected:
    int posAbs;
    float posRel;

signals:
    void ValueChangedAbs(int absPos);
    void ValueChangedRel(float relPos);

private:
    Ui::HorSliderSelfCentering *ui;
    QTimer timer;

private slots:
    void Slot_ValueChanged(int);
    void Slot_HandleReleased();
    void Slot_TimerExpired();
};

#endif // HORSLIDERSELFCENTERING_H
