#ifndef CHANGESLIDERATRATE_H
#define CHANGESLIDERATRATE_H

#include <QSlider>
#include <QDebug>

static int instCntr = 0;

class ChangeSliderAtRate
{
public:
    ChangeSliderAtRate()
    {
        rate = step = 0.0;
        inst = instCntr;
        instCntr++;
    }

    void Ping()
    {
        if(rate == 0.0) return;
        step += rate;
        if((step >= 1.0) || (step <= -1.0))
        {
            int intStep = (int)step;
            mySlider->setValue(mySlider->value()+intStep);
            step -= intStep;
            if(mySlider->value() == target)rate = 0;
        }
    }

    void SetValue(int _target)
    {
        target = _target;
        int diff = target - mySlider->value();
        rate = ((float)diff) / 200.0;
    }

    QSlider* mySlider;

private:
    float rate, step;
    int target;
    int inst;
};

#endif // CHANGESLIDERATRATE_H
